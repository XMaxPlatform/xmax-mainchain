#include <contractutil_plugin.hpp>

namespace xmax {
	using namespace xmaxapp;

	/**
	*  Implementation details of the contract util plugin
	*
	*/
	class ContractUtilPluginImpl {	
	public:
		ContractUtilPluginImpl() {}
		~ContractUtilPluginImpl();
	};

	/**
	*  Implementations of ContractUtilPluginImpl interfaces
	*
	*/
	ContractUtilPluginImpl::~ContractUtilPluginImpl()
	{
	}


	/**
	*  Implementations of ContractUtilPlugin interfaces
	*
	*/
	//--------------------------------------------------
	ContractUtilPlugin::ContractUtilPlugin() {	
		// TODO : initialize members
	}

	//--------------------------------------------------
	ContractUtilPlugin::~ContractUtilPlugin() {	

	}

	//--------------------------------------------------
	void ContractUtilPlugin::Initialize(const xmaxapp::VarsMap& options) {
		
		PluginFace::Initialize(options);

		impl_.reset(new ContractUtilPluginImpl());

		test();
	}

	//--------------------------------------------------
	void ContractUtilPlugin::Startup() {	
		PluginFace::Startup();
	}


	//--------------------------------------------------
	void ContractUtilPlugin::Shutdown() {	
		impl_.reset();


		PluginFace::Shutdown();
	}

}
