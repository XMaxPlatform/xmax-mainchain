#include <contractutil_plugin.hpp>
#include <google/protobuf/stubs/common.h>

namespace xmax {
	using namespace xmaxapp;

	/**
	*  Implementation details of the contract util plugin
	*
	*/
	class ContractUtilPluginImpl {	
	public:
		XmaxNetPluginImpl() {}
		~XmaxNetPluginImpl();
	};

	/**
	*  Implementations of ContractUtilPluginImpl interfaces
	*
	*/
	ContractUtilPluginImpl::~ContractUtilPluginImpl()
	{
		ShutdownProtobufLibrary();
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

}
