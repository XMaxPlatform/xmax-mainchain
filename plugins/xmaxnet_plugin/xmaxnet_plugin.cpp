#include <xmaxnet_plugin.hpp>


namespace Xmax {
	using namespace Xmaxapp;

	/**
	*  Implementation details of the xmax net plugin
	*
	*/
	class xmaxnet_plugin_impl
	{
	public:
		xmaxnet_plugin_impl() {}
		~xmaxnet_plugin_impl() {}
	};



	/**
	*  Implementations of xmaxnet_plugin interfaces
	*
	*/
	//--------------------------------------------------
	Xmax::xmaxnet_plugin::xmaxnet_plugin()
	{

	}

	//--------------------------------------------------
	Xmax::xmaxnet_plugin::~xmaxnet_plugin()
	{

	}

	//--------------------------------------------------
	void Xmax::xmaxnet_plugin::initialize(const vars_map& options)
	{
		m_pImpl.reset(new xmaxnet_plugin_impl());
	}

	//--------------------------------------------------
	void Xmax::xmaxnet_plugin::startup()
	{

	}


	//--------------------------------------------------
	void xmaxnet_plugin::shutdown()
	{
		m_pImpl.reset();
	}

}
