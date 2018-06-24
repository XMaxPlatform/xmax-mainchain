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
		void Init(boost::asio::io_service& io);
		/**
		* set endpoint (using boost params)
		* @param[in]	std::string		endpoint of network
		*/
		void SetEndpoint(const std::string& endpoint);


	protected:

		void OnAccept();

		void OnConnect();

		void OnResolve();

	private:

		std::unique_ptr<tcp::acceptor>		acceptor_;
		std::unique_ptr<tcp::resolver>		resolver_;
		tcp::endpoint						endpoint_;
		std::vector<XMX_Connection>			connections_;
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
	void XmaxNetPluginImpl::Init(boost::asio::io_service& io)
	{
		resolver_ = std::make_unique<tcp::resolver>( io );
		acceptor_.reset( new tcp::acceptor(io) );
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
		impl_->Init(*GetApp()->GetService());
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
