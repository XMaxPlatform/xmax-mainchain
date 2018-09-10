#include "xmaxnet_plugin.hpp"
#include "xmaxnet_pluginimpl.hpp"
#include "netmsg_processing.h"



namespace xmax {
	using namespace xmaxapp;
	using namespace pro;
	

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

		impl_ = std::make_shared<XmaxNetPluginImpl>(*GetApp()->GetService());
		impl_->Init(options);
		NetMsgProcessing::GetInstance().SetNetImpl(impl_);
	}

	//--------------------------------------------------
	void XmaxNetPlugin::Startup() 
	{	
		PluginFace::Startup();
		impl_->Startup();
	}


	//--------------------------------------------------
	void XmaxNetPlugin::Shutdown() 
	{	

		PluginFace::Shutdown();
	}

	void XmaxNetPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		cfg.add_options()
			(XmaxNetPluginImpl::s_ServerAddress, xmaxapp::options::value<std::string>()->default_value("127.0.0.1:8080"), "default server address(ip:port)");

		cfg.add_options()
			(XmaxNetPluginImpl::s_PeerAddress, xmaxapp::options::value<std::vector<std::string>>()->composing(), "list of peer address");

		cfg.add_options()
			(XmaxNetPluginImpl::s_UPNP, xmaxapp::options::value<bool>()->default_value(false), "turn on/off upnp\n");
	}

}
