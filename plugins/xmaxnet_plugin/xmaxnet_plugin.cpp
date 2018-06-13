#include <xmaxnet_plugin.hpp>


namespace Xmax {
	using namespace Xmaxapp;

	/**
	*  Implementation details of the xmax net plugin
	*
	*/
	class XmaxNetPluginImpl
	{
	public:
		XmaxNetPluginImpl() {}
		~XmaxNetPluginImpl() {}
	};



	/**
	*  Implementations of xmaxnet_plugin interfaces
	*
	*/
	//--------------------------------------------------
	Xmax::XmaxNetPlugin::XmaxNetPlugin()
	{

	}

	//--------------------------------------------------
	Xmax::XmaxNetPlugin::~XmaxNetPlugin()
	{

	}

	//--------------------------------------------------
	void Xmax::XmaxNetPlugin::initialize(const vars_map& options)
	{
		impl_.reset(new XmaxNetPluginImpl());
	}

	//--------------------------------------------------
	void Xmax::XmaxNetPlugin::startup()
	{

	}


	//--------------------------------------------------
	void XmaxNetPlugin::shutdown()
	{
		impl_.reset();
	}

}
