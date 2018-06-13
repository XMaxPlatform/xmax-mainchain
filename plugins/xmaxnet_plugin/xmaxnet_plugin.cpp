#include <xmaxnet_plugin.hpp>


namespace xmax {
	using namespace xmaxapp;

	/**
	*  Implementation details of the xmax net plugin
	*
	*/
	class XmaxNetPluginImpl {	
	public:
		XmaxNetPluginImpl() {}
		~XmaxNetPluginImpl() {}
	};



	/**
	*  Implementations of xmaxnet_plugin interfaces
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
