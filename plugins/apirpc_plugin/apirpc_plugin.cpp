#include "apirpc_plugin.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include "pro/log/log.hpp"

using namespace std;
using namespace boost::beast;

namespace bio = boost::asio;
using tcp = bio::ip::tcp;

namespace xmax {

	static const char* const kHttpApiAddressOp = "http-api-address";
	static const char* const kAllowCrossOriginOp = "api-allow-cross-origin";
	
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

	public:
		explicit HttpSession(tcp::socket socket);

		void Run();
		void DoRead();
		void Close();

		//Events
		void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);		

	private:
		tcp::socket socket_;
		http::request<http::string_body> request_;
		boost::beast::flat_buffer buffer_;
		boost::asio::strand<boost::asio::io_context::executor_type> strand_;
	};


	//--------------------------------------------------
	HttpSession::HttpSession(tcp::socket socket):
		socket_(std::move(socket)),
		strand_(socket.get_executor())
	{

	}

	//--------------------------------------------------
	void HttpSession::Run()
	{
		LogSprintf("Http session run.");
	}


	//--------------------------------------------------
	void HttpSession::DoRead()
	{
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
		
	}

	/*
	* Http listener class
	*/
	class HttpListener: public std::enable_shared_from_this<HttpListener> {
	public:
		HttpListener(bio::io_context& ioc, tcp::endpoint endpoint);

		//Public interfaces
		void Run();
		void DoAccept();

		//Events
		void OnAccept(boost::system::error_code ec);

	private:
		tcp::acceptor acceptor_;
		tcp::socket socket_;
	};

	HttpListener::HttpListener(bio::io_context& ioc, tcp::endpoint endpoint) :
		acceptor_(ioc),
		socket_(ioc)
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
			std::make_shared<HttpSession>(std::move(socket_))->Run();
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

		ApiRpcPluginImpl();
		~ApiRpcPluginImpl();

		void Initialization();

		/*
		* Start the io service to listen to the http request		
		*/
		void Start();

	public:
		//Configurations
		string allow_cross_origin;
		string http_api_address;

		//IO
		boost::asio::io_context ioc;
		std::shared_ptr<HttpListener> listener;
		unsigned short http_port;
		bio::ip::address http_address;

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
		LogSprintf("Start API RPC service.");
	
		//listener = std::make_shared<http_listener>(ioc, tcp::endpoint{ http_address, http_port});
		//listener->Run();
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


	}


	//--------------------------------------------------
	ApiRpcPlugin::ApiRpcPlugin()
	{

	}


	//--------------------------------------------------
	ApiRpcPlugin::~ApiRpcPlugin()
	{

	}

	//--------------------------------------------------
	void ApiRpcPlugin::Initialize(const VarsMap& options)
	{
		impl_ = std::make_unique<ApiRpcPluginImpl>();

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
		impl_.reset();
	}

	//--------------------------------------------------
	void ApiRpcPlugin::Startup()
	{
		impl_->Start();
	}

}


