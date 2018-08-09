#include "apirpc_plugin.hpp"
#include "boost/asio/io_context.hpp"
#include "pro/log/log.hpp"

using namespace std;

namespace xmax {

	static const char* const kHttpApiAddressOp = "http-api-address";
	static const char* const kAllowCrossOriginOp = "api-allow-cross-origin";

	/*!
	 * \class ApiRpcPluginImpl
	 *
	 * \brief Implementation details of ApiRpcPlugin
	 *
	 */
	class ApiRpcPluginImpl {
	public:

		ApiRpcPluginImpl();
		~ApiRpcPluginImpl();

		void Initialization();

		/*
		* Start the io service to listen to the http request		
		*/
		void Start();

	public:
		//Configurations
		string allow_cross_origin;
		string http_api_address;

		//IO
		boost::asio::io_context ioc;

	};

	//--------------------------------------------------
	ApiRpcPluginImpl::ApiRpcPluginImpl()
	{

	}

	//--------------------------------------------------
	ApiRpcPluginImpl::~ApiRpcPluginImpl()
	{
		
	}


	//--------------------------------------------------
	void ApiRpcPluginImpl::Initialization()
	{
		auto addr = boost::asio::ip::address::from_string(http_api_address);		
	}

	//--------------------------------------------------
	void ApiRpcPluginImpl::Start()
	{
		LogSprintf("Start API RPC service.");
	
		
	}

	/*!
	* \class ApiRpcPlugin
	*
	* \brief Implementation details of ApiRpcPlugin
	*
	*/
	//--------------------------------------------------
	void ApiRpcPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		cfg.add_options()
			(kHttpApiAddressOp, xpo::value<std::string>()->default_value("127.0.0.1:8081"), "default api rpc http address(ip:port)");

		cfg.add_options()
			(kAllowCrossOriginOp, xpo::value<std::string>()->default_value("*"), "default api rpc http address(ip:port)");


	}


	//--------------------------------------------------
	ApiRpcPlugin::ApiRpcPlugin()
	{

	}


	//--------------------------------------------------
	ApiRpcPlugin::~ApiRpcPlugin()
	{

	}

	//--------------------------------------------------
	void ApiRpcPlugin::Initialize(const VarsMap& options)
	{
		impl_ = std::make_unique<ApiRpcPluginImpl>();

		if (options.count(kAllowCrossOriginOp)) {
			impl_->allow_cross_origin = options.at(kAllowCrossOriginOp).as<string>();
		}

		if (options.count(kHttpApiAddressOp)) {
			impl_->http_api_address = options.at(kHttpApiAddressOp).as<string>();
		}

		impl_->Initialization();
	}

	//--------------------------------------------------
	void ApiRpcPlugin::Shutdown()
	{
		impl_.reset();
	}

	//--------------------------------------------------
	void ApiRpcPlugin::Startup()
	{
		impl_->Start();
	}

}



