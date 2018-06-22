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

	}

	//--------------------------------------------------
	ContractUtilPlugin::~ContractUtilPlugin() {	

	}

	//--------------------------------------------------
	void ContractUtilPlugin::Initialize(const xmaxapp::VarsMap& options) {
		
		PluginFace::Initialize(options);

		impl_.reset(new ContractUtilPluginImpl());
	}
	

}
