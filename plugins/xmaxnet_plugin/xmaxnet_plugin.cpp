#include <xmaxnet_plugin.hpp>
#include <google/protobuf/stubs/common.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/host_name.hpp>

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

}
