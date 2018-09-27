#include "apirpc_plugin.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include "pro/log/log.hpp"
#include <functional>

using namespace std;
using namespace boost::beast;

namespace bio = boost::asio;
using tcp = bio::ip::tcp;
using HttpHandlerFunc = std::function<std::optional<http::response<http::string_body>>(boost::beast::string_view url, http::request<http::string_body>& req)>;

namespace xmax {

	static const char* const kHttpApiAddressOp = "http-api-address";
	static const char* const kAllowCrossOriginOp = "api-allow-cross-origin";
	static const char* const kAllowCendentialOp = "access_control_allow_credentials";
	
	namespace {
		/*
		* Extract IP and port pair from ip address with format like "127.0.0.1:1234"
		*/
		static std::tuple<std::string, unsigned short> ParseIPAddress(const std::string& addr_str) {
			auto colon_idx = addr_str.find_last_of(":");
			assert(colon_idx != std::string::npos);			
			std::string port_str = addr_str.substr(colon_idx + 1);
			std::stringstream ss;
			ss << port_str;
			unsigned short port = 0;
			ss >> port;
			std::string ip_str = addr_str.substr(0, colon_idx);
			return std::make_tuple(ip_str, port);
		}

		
		
	}

	


	/*
	* Http session
	*/
	class HttpSession : public std::enable_shared_from_this<HttpSession> {

		class Queue {
			enum {
				kQueueLimit = 10
			};

			struct Work {
				virtual ~Work() = default;
				virtual void operator()() = 0;
			};

		public:
			explicit Queue(HttpSession& s) :session_{ s } {
				static_assert(kQueueLimit > 0, "Http session queue limit need above 0.");
				items_.reserve(kQueueLimit);
			}

			template<bool isRequest, class Body, class Fields>
			void operator()(http::message<isRequest, Body, Fields>&& msg) {
				static_assert(kQueueLimit > 0, "Http session queue limit need above 0.");

				struct WorkImpl : Work {

					HttpSession& session_;
					http::message<isRequest, Body, Fields> msg_;
					
					WorkImpl(HttpSession& session, http::message<isRequest, Body, Fields>&& msg) :
						session_(session), msg_(std::move(msg)) {

					}

					void operator()() {
						http::async_write(
							session_.socket_,
							msg_,
							boost::asio::bind_executor(
								session_.strand_,
								std::bind(
									&HttpSession::OnWrite,
									session_.shared_from_this(),
									std::placeholders::_1,
									msg_.need_eof())));
					}
					
				};

				// Queue the message
				items_.push_back(boost::make_unique<WorkImpl>(session_, std::move(msg)));

				// Execute the front work
				if (items_.size() == 1)
					(*items_.front())();

			}

			// Called when a message finishes sending
			// Returns `true` if the caller should initiate a read
			bool OnWrite() {
				assert(!items_.empty());

				bool wasFull = IsFull();

				items_.erase(items_.begin());
				if (!items_.empty())
					(*items_.front())();


				return wasFull;
			}

			bool IsFull() const
			{
				return items_.size() >= kQueueLimit;
			}

		private:
			HttpSession& session_;
			std::vector<std::unique_ptr<Work>> items_;			
		};

	public:
		explicit HttpSession(tcp::socket socket, HttpHandlerFunc http_handler);

		void Run();
		void DoRead();
		void Close();

		template <class Send>
		void HandleRequest(Send&& send);

		//Events
		void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);	
		void OnWrite(boost::system::error_code ec, bool close);
		void OnTimer(boost::system::error_code ec);

	private:
		bool IsValidRequestVerb(const http::verb& req_method) const {
			return req_method == http::verb::get || req_method == http::verb::head || req_method == http::verb::post;
		}

		bool IsValidRequestTarget(boost::beast::string_view request_target) {
			return !(request_target.empty() ||
				request_target[0] != '/' ||
				request_target.find("..") != boost::beast::string_view::npos ||
				request_target.back() == '/');
		}

		void DoClose();

	private:
		tcp::socket socket_;
		http::request<http::string_body> request_;
		boost::beast::flat_buffer buffer_;
		boost::asio::strand<boost::asio::io_context::executor_type> strand_;
		Queue queue_;
		HttpHandlerFunc http_handler_;
		boost::asio::steady_timer timer_;
	};


	//--------------------------------------------------
	HttpSession::HttpSession(tcp::socket socket, HttpHandlerFunc http_handler):
		socket_(std::move(socket)),
		strand_(socket.get_executor()),
		queue_(*this),
		http_handler_(http_handler),
		timer_(socket.get_executor().context(), (std::chrono::steady_clock::time_point::max)())
	{

	}

	//--------------------------------------------------
	void HttpSession::Run()
	{
		LogSprintf("Http session run.");

		DoRead();
	}


	//--------------------------------------------------
	void HttpSession::DoRead()
	{
		request_ = {};

		http::async_read(socket_, buffer_, request_,
			bio::bind_executor(strand_,
				std::bind(&HttpSession::OnRead, shared_from_this(),
					std::placeholders::_1,
					std::placeholders::_2)));
	}


	//--------------------------------------------------
	void HttpSession::Close()
	{
		// Send a TCP shutdown
		boost::system::error_code ec;
		socket_.shutdown(tcp::socket::shutdown_send, ec);

	}

	//--------------------------------------------------
	template <class Send>
	void HttpSession::HandleRequest(Send&& send)
		{
		auto& req = request_;

		// Returns a bad request response
		auto const bad_request =
			[&req](boost::beast::string_view why)
		{
			http::response<http::string_body> res{ http::status::bad_request, req.version() };
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = why.to_string();
			res.prepare_payload();
			return res;
		};

		// Returns a not found response
		auto const not_found =
			[&req](boost::beast::string_view target)
		{
			http::response<http::string_body> res{ http::status::not_found, req.version() };
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = "The resource '" + target.to_string() + "' was not found.";
			res.prepare_payload();
			return res;
		};

		// Returns a server error response
		auto const server_error =
			[&req](boost::beast::string_view what)
		{
			http::response<http::string_body> res{ http::status::internal_server_error, req.version() };
			res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
			res.set(http::field::content_type, "text/html");
			res.keep_alive(req.keep_alive());
			res.body() = "An error occurred: '" + what.to_string() + "'";
			res.prepare_payload();
			return res;
		};
		
		if (!IsValidRequestVerb(req.method())) {
			return send(bad_request("Invalid http method."));
		}

		// Request path must be absolute and not contain "..".
		if (!IsValidRequestTarget(req.target()))
			return send(bad_request("Invalid request target."));

		if (http_handler_)
		{
			auto res = http_handler_(req.target(), req);
			if (res) {
				return send(std::move(*res));
			}
		}

		
		return send(not_found(req.target()));
	}

	//--------------------------------------------------
	void HttpSession::OnRead(boost::system::error_code ec,
		std::size_t bytes_transferred)
	{

		if (ec == bio::error::operation_aborted)
		{			
			return;
		}

		if (ec == http::error::end_of_stream)
		{
			Close();
			return;
		}

		if (ec)
		{
			ErrorSprintf("Http session on read failed with error message:%s", ec.message().c_str());
			return;
		}

		HandleRequest(queue_);
		
	}


	//--------------------------------------------------
	void HttpSession::OnWrite(boost::system::error_code ec, bool close)
	{
		// Happens when the timer closes the socket
		if (ec == boost::asio::error::operation_aborted)
			return;

		if (ec) {
			ErrorSprintf("Http session on write failed with error message:%s", ec.message().c_str());
			return;
		}

		if (close)
		{
			DoClose();
		}

		if (queue_.OnWrite())
		{
			DoRead();
		}
	}


	//--------------------------------------------------
	void HttpSession::OnTimer(boost::system::error_code ec)
	{
		if (ec && ec != boost::asio::error::operation_aborted) {
			ErrorSprintf("Http session on timer failed with error message:%s", ec.message().c_str());
			return;
		}

		// Verify that the timer really expired since the deadline may have moved.
		if (timer_.expiry() <= std::chrono::steady_clock::now())
		{
			// Closing the socket cancels all outstanding operations. They
			// will complete with boost::asio::error::operation_aborted
			socket_.shutdown(tcp::socket::shutdown_both, ec);
			socket_.close(ec);
			return;
		}

		timer_.async_wait(
			boost::asio::bind_executor(
				strand_,
				std::bind(
					&HttpSession::OnTimer,
					shared_from_this(),
					std::placeholders::_1)));
			
	}

	//--------------------------------------------------
	void HttpSession::DoClose()
	{
		boost::system::error_code ec;
		socket_.shutdown(tcp::socket::shutdown_send, ec);
	}

	/*
	* Http listener class
	*/
	class HttpListener: public std::enable_shared_from_this<HttpListener> {
	public:
		HttpListener(bio::io_context& ioc, tcp::endpoint endpoint, HttpHandlerFunc http_handler);

		//Public interfaces
		void Run();
		void DoAccept();

		//Events
		void OnAccept(boost::system::error_code ec);

	private:
		tcp::acceptor acceptor_;
		tcp::socket socket_;
		HttpHandlerFunc http_handler_;
	};

	HttpListener::HttpListener(bio::io_context& ioc, tcp::endpoint endpoint, HttpHandlerFunc http_handler) :
		acceptor_(ioc),
		socket_(ioc),
		http_handler_(http_handler)
	{
		boost::system::error_code ec;

		// Open the acceptor
		acceptor_.open(endpoint.protocol(), ec);
		if (ec)
		{
			ErrorSprintf("Open http acceptor failed with error message:%s", ec.message().c_str());
			return;
		}

		// Allow address reuse
		acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
		if (ec)
		{
			ErrorSprintf("Http acceptor set_option failed with error message:%s", ec.message().c_str());
			return;
		}

		// Bind to the server address
		acceptor_.bind(endpoint, ec);
		if (ec)
		{
			ErrorSprintf("Http acceptor bind endpoint failed with error message:%s", ec.message().c_str());
			return;
		}

	}

	//--------------------------------------------------
	void HttpListener::Run()
	{
		

		if (!acceptor_.is_open())
		{
			return;
		}

		DoAccept();
	}


	//--------------------------------------------------
	void HttpListener::DoAccept()
	{
		acceptor_.async_accept(
			socket_,
			std::bind(
				&HttpListener::OnAccept,
				shared_from_this(),
				std::placeholders::_1));
	
	}


	//--------------------------------------------------
	void HttpListener::OnAccept(boost::system::error_code ec)
	{
		if (ec) {
			ErrorSprintf("HttpListener OnAccpet occurs a error message:%s", ec.message().c_str());
			return;	
		}
		else {
			//TODO: Run sesson
			std::make_shared<HttpSession>(std::move(socket_), http_handler_)->Run();
		}

		DoAccept();
	}

	/*!
	 * \class ApiRpcPluginImpl
	 *
	 * \brief Implementation details of ApiRpcPlugin
	 *
	 */
	class ApiRpcPluginImpl {
	public:
		using UrlHandlers = std::unordered_map<std::string, UrlHandler>;
	public:

		ApiRpcPluginImpl();
		~ApiRpcPluginImpl();

		void Initialization();

		/*
		* Start the io service to listen to the http request		
		*/
		void Start();


		void AddUrlHandler(std::string_view url, UrlHandler handler) {
			url_handlers_[std::string{ url }] = handler;
		}

	private:
		//Http handler callback
		std::optional<http::response<http::string_body>> HttpHandler(boost::beast::string_view url, http::request<http::string_body>& req);

	public:
		//Configurations
		string allow_cross_origin;
		string http_api_address;
		string allow_credentials;

		//IO
		boost::asio::io_context ioc;
		std::shared_ptr<HttpListener> listener;
		unsigned short http_port;
		bio::ip::address http_address;

		//Other
		UrlHandlers url_handlers_;
	};

	//--------------------------------------------------
	ApiRpcPluginImpl::ApiRpcPluginImpl()
	{

	}

	//--------------------------------------------------
	ApiRpcPluginImpl::~ApiRpcPluginImpl()
	{

		listener.reset();
	}


	//--------------------------------------------------
	void ApiRpcPluginImpl::Initialization()
	{
		using namespace boost::asio;

		auto addr = ip::address::from_string(http_api_address);

		auto[ip_str, port] = ParseIPAddress(http_api_address);		

		http_address = bio::ip::make_address(ip_str);
		http_port = port;
	}

	//--------------------------------------------------
	void ApiRpcPluginImpl::Start()
	{
		using namespace std::placeholders;
		LogSprintf("Start API RPC service.");
	
		listener = std::make_shared<HttpListener>(ioc, tcp::endpoint{ http_address, http_port}, std::bind(&ApiRpcPluginImpl::HttpHandler, this, _1, _2));
		//listener->Run();
	}


	//--------------------------------------------------
	std::optional<http::response<http::string_body>> ApiRpcPluginImpl::HttpHandler(boost::beast::string_view url, http::request<http::string_body>& req)
	{
		std::optional<http::response<http::string_body>> res;

		std::string url_str{ url };
		auto itr = url_handlers_.find(url_str);
		if (itr != url_handlers_.end())
		{
			auto handler_res = itr->second(url.to_string(), req.body());
			if (handler_res)
			{
				auto[status, body] = handler_res.value();
				res.emplace((boost::beast::http::status)status, req.version());
				res->set(http::field::server, BOOST_BEAST_VERSION_STRING);
				if (!allow_cross_origin.empty())
				{
					res->set(http::field::access_control_allow_origin, allow_cross_origin);
				}			
				
				if (!allow_credentials.empty())
				{
					res->set(http::field::access_control_allow_credentials, allow_credentials);
				}
				
				res->set(http::field::content_type, "application/json");
				res->body() = body;
			}
		}	

	
		return res;
	}

	/*!
	* \class ApiRpcPlugin
	*
	* \brief Implementation details of ApiRpcPlugin
	*
	*/
	//--------------------------------------------------
	void ApiRpcPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		cfg.add_options()
			(kHttpApiAddressOp, xpo::value<std::string>()->default_value("127.0.0.1:8081"), "default api rpc http address(ip:port)");

		cfg.add_options()
			(kAllowCrossOriginOp, xpo::value<std::string>()->default_value("*"), "default api rpc http address(ip:port)");


		cfg.add_options()
			(kAllowCendentialOp, xpo::value<std::string>()->default_value("true"), "allow credentials access control.");
	}


	//--------------------------------------------------
	ApiRpcPlugin::ApiRpcPlugin()
	{
		impl_ = std::make_unique<ApiRpcPluginImpl>();
	}


	//--------------------------------------------------
	ApiRpcPlugin::~ApiRpcPlugin()
	{
		impl_.reset();
	}

	//--------------------------------------------------
	void ApiRpcPlugin::Initialize(const VarsMap& options)
	{
		
		if (options.count(kAllowCrossOriginOp)) {
			impl_->allow_cross_origin = options.at(kAllowCrossOriginOp).as<string>();
		}

		if (options.count(kHttpApiAddressOp)) {
			impl_->http_api_address = options.at(kHttpApiAddressOp).as<string>();
		}

		impl_->Initialization();
	}

	//--------------------------------------------------
	void ApiRpcPlugin::Shutdown()
	{
		
	}


	//--------------------------------------------------
	void ApiRpcPlugin::AddUrlHandler(std::string_view url, UrlHandler handler)
	{
		impl_->AddUrlHandler(url, handler);
	}

	//--------------------------------------------------
	void ApiRpcPlugin::Startup()
	{
		impl_->Start();
	}

}



