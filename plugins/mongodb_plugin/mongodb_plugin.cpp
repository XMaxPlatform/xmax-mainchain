#include <mongodb_plugin.hpp>

namespace Xmax {
	using namespace Xmaxapp;

	/**
	*  Implementation details of the xmax mongodb plugin
	*
	*/
	class MongoDBPluginImpl
	{
	public:
		MongoDBPluginImpl() {}
		~MongoDBPluginImpl() {}


	};



	/**
	*  Implementations of xmaxnet_plugin interfaces
	*
	*/
	//--------------------------------------------------
	Xmax::MongoDBPlugin::MongoDBPlugin()
	{

	}

	//--------------------------------------------------
	Xmax::MongoDBPlugin::~MongoDBPlugin()
	{

	}

	//--------------------------------------------------
	void Xmax::MongoDBPlugin::initialize(const vars_map& options)
	{
		impl_.reset(new MongoDBPluginImpl());
	}

	//--------------------------------------------------
	void Xmax::MongoDBPlugin::startup()
	{

	}


	//--------------------------------------------------
	void MongoDBPlugin::shutdown()
	{
		impl_.reset();
	}

}
