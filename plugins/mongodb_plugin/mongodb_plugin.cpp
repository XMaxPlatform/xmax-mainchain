#include <mongodb_plugin.hpp>

namespace Xmax {
	using namespace xmaxapp;

	/**
	*  Implementation details of the xmax mongodb plugin
	*
	*/
	class MongoDBPluginImpl {	
	public:
		MongoDBPluginImpl() {}
		~MongoDBPluginImpl() {}


	};



	/**
	*  Implementations of xmaxnet_plugin interfaces
	*
	*/
	//--------------------------------------------------
	Xmax::MongoDBPlugin::MongoDBPlugin() {	

	}

	//--------------------------------------------------
	Xmax::MongoDBPlugin::~MongoDBPlugin() {	

	}

	//--------------------------------------------------
	void Xmax::MongoDBPlugin::Initialize(const xmaxapp::VarsMap& options) {
		PluginFace::Initialize(options);

		impl_.reset(new MongoDBPluginImpl());
	}

	//--------------------------------------------------
	void Xmax::MongoDBPlugin::Startup() {	
		PluginFace::Startup();
	}


	//--------------------------------------------------
	void MongoDBPlugin::Shutdown() {	
		impl_.reset();



		PluginFace::Shutdown();
	}

}
