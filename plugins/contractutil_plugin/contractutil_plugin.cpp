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
		//  : initialize members
	}

	/*
	*  desconstract util plugin
	*/
	//--------------------------------------------------
	ContractUtilPlugin::~ContractUtilPlugin() {
		Clean();
	}

	/*
		initialize the contract
	*/
	//--------------------------------------------------
	void ContractUtilPlugin::Initialize(const xmaxapp::VarsMap& options) {

		PluginFace::Initialize(options);

		impl_.reset(new ContractUtilPluginImpl());

		// test initialize code 
		Test("init");
	}
	
	/*
	*  startup a contract util plugin
	* 
	*/
	//--------------------------------------------------
	void ContractUtilPlugin::Startup() {
		PluginFace::Startup();
		
		//  : setup all 
		CreateContract();
	}

	/*
	*	shutdown the contract
	*/
	//--------------------------------------------------
	void ContractUtilPlugin::Shutdown() {
		impl_.reset();

		PluginFace::Shutdown();
	}

	/*
	*  test for contract
	*/
	//--------------------------------------------------
	void ContractUtilPlugin::Test(const char* log) {
		printf("test a log");
	}

	/*
	* clean contract
	*/
	//--------------------------------------------------
	void ContractUtilPlugin::Clean()
	{
		printf("clean");
	}

	/*
	*	create contract 
	*/
	//-------------------------------------------------
	void ContractUtilPlugin::CreateContract()
	{
		// create contract
		printf("create contract {}");
	}

	/*
	*
	*/
	//--------------------------------------------------
	void ContractUtilPlugin::FreeAllContracts()
	{

	}
}
