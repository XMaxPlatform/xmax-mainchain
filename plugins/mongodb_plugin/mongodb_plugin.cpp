#include <mongodb_plugin.hpp>

namespace Xmax {
	using namespace Xmaxapp;

	/**
	*  Implementation details of the xmax mongodb plugin
	*
	*/
	class mongodb_plugin_impl
	{
	public:
		mongodb_plugin_impl() {}
		~mongodb_plugin_impl() {}


	};



	/**
	*  Implementations of xmaxnet_plugin interfaces
	*
	*/
	//--------------------------------------------------
	Xmax::mongodb_plugin::mongodb_plugin()
	{

	}

	//--------------------------------------------------
	Xmax::mongodb_plugin::~mongodb_plugin()
	{

	}

	//--------------------------------------------------
	void Xmax::mongodb_plugin::initialize(const vars_map& options)
	{
		m_pImpl.reset(new mongodb_plugin_impl());
	}

	//--------------------------------------------------
	void Xmax::mongodb_plugin::startup()
	{

	}


	//--------------------------------------------------
	void mongodb_plugin::shutdown()
	{
		m_pImpl.reset();
	}

}
