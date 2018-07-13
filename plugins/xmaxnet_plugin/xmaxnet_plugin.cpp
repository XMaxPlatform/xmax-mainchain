#include <xmaxnet_plugin.hpp>
#include <google/protobuf/stubs/common.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>

#include "xmx_connection.hpp"

namespace xmax {
	using namespace xmaxapp;
	using namespace google::protobuf;
	using boost::asio::ip::tcp;
	using boost::asio::ip::address_v4;
	using boost::asio::ip::host_name;

	const char* s_ServerAddress = "server-address";


	/**
	*  Implementation details of the xmax net plugin
	*
	*/
	class XmaxNetPluginImpl {	
	public:
		XmaxNetPluginImpl() {}
		~XmaxNetPluginImpl();

	public:
		/**
		* Init p2p newwork params
		*/
		void Init(boost::asio::io_service& io, const VarsMap& options);
		/**
		* set endpoint (using boost params)
		* @param[in]	std::string		endpoint of network
		*/
		void SetEndpoint(const std::string& endpoint);

		void Connect(std::shared_ptr<XMX_Connection> pConnect);

		void Connect(std::shared_ptr<XMX_Connection> pConnect, tcp::resolver::iterator endpointItr);
		
		/**
		* call back of socket accept
		*/
		void OnAccept();

		void OnConnect();

		void OnResolve();
		/**
		*  start a session
		*/
		void StartSession(std::shared_ptr<XMX_Connection> pConnect);
		/**
		*  start listening loop
		*/
		void StartListen();
		/**
		*  async read a message
		*/
		void StartReadMsg(std::shared_ptr<XMX_Connection> pConnect);
		/**
		*  handle various msg
		*/
		void HandleMsg();

	private:

		std::unique_ptr<tcp::acceptor>		acceptor_;
		std::unique_ptr<tcp::resolver>		resolver_;
		tcp::endpoint						endpoint_;
		std::vector<XMX_Connection>			connections_;
		uint								nClients_;
		std::string							seedServer_;
	};

	/**
	*  Implementations of XmaxNetPluginImpl interfaces
	*
	*/
	XmaxNetPluginImpl::~XmaxNetPluginImpl()
	{
		ShutdownProtobufLibrary();
	}
	/**
	*  Initialize network
	*
	*/
	void XmaxNetPluginImpl::Init(boost::asio::io_service& io, const VarsMap& options)
	{
		resolver_ = std::make_unique<tcp::resolver>( io );
		acceptor_.reset( new tcp::acceptor(io) );

		if (options.count(s_ServerAddress))
		{
			seedServer_		 = options.at(s_ServerAddress).as<std::string>();
			size_t spos		 = seedServer_.find(':');
			std::string addr = seedServer_.substr(0, spos);
			std::string port = seedServer_.substr(spos + 1, seedServer_.size());

			tcp::resolver::query query(tcp::v4(), addr.c_str(), port.c_str());
			endpoint_ = *resolver_->resolve(query);
			acceptor_.reset(new tcp::acceptor(io));
		}
	}

	void XmaxNetPluginImpl::SetEndpoint(const std::string& endpoint)
	{
		
	}

	void XmaxNetPluginImpl::OnAccept()
	{

	}

	void XmaxNetPluginImpl::OnConnect()
	{

	}

	void XmaxNetPluginImpl::OnResolve()
	{

	}

	void XmaxNetPluginImpl::StartSession(std::shared_ptr<XMX_Connection> pConnect)
	{

	}

	void XmaxNetPluginImpl::StartListen()
	{

	}

	void XmaxNetPluginImpl::StartReadMsg(std::shared_ptr<XMX_Connection> pConnect)
	{

	}

	void XmaxNetPluginImpl::HandleMsg()
	{

	}

	void XmaxNetPluginImpl::Connect(std::shared_ptr<XMX_Connection> pConnect)
	{

	}

	void XmaxNetPluginImpl::Connect(std::shared_ptr<XMX_Connection> pConnect, tcp::resolver::iterator endpointItr)
	{

	}


	/**
	*  Implementations of XmaxNetPlugin interfaces
	*
	*/
	//--------------------------------------------------
	XmaxNetPlugin::XmaxNetPlugin() {	

	}

	//--------------------------------------------------
	XmaxNetPlugin::~XmaxNetPlugin() {	

	}

	//--------------------------------------------------
	void XmaxNetPlugin::Initialize(const xmaxapp::VarsMap& options) {
		
		PluginFace::Initialize(options);

		impl_.reset(new XmaxNetPluginImpl());
		impl_->Init(*GetApp()->GetService(), options);
	}

	//--------------------------------------------------
	void XmaxNetPlugin::Startup() {	
		PluginFace::Startup();
	}


	//--------------------------------------------------
	void XmaxNetPlugin::Shutdown() {	
		impl_.reset();


		PluginFace::Shutdown();
	}

	void XmaxNetPlugin::Connect(const std::string& endPoint)
	{

	}


}
