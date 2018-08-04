#include "apirpc_plugin.hpp"

using namespace std;

namespace xmax {

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


	};

	//--------------------------------------------------
	ApiRpcPluginImpl::ApiRpcPluginImpl()
	{

	}

	//--------------------------------------------------
	ApiRpcPluginImpl::~ApiRpcPluginImpl()
	{

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
			("http-api-address", xpo::value<std::string>()->default_value("127.0.0.1:8081"), "default api rpc http address(ip:port)");

		cfg.add_options()
			("api-allow-cross-origin", xpo::value<std::string>()->default_value("*"), "default api rpc http address(ip:port)");


	}

	//--------------------------------------------------
	void ApiRpcPlugin::Initialize(const VarsMap& options)
	{
		impl_ = std::make_unique<ApiRpcPluginImpl>();
	}

	//--------------------------------------------------
	void ApiRpcPlugin::Shutdown()
	{
		impl_.reset();
	}

	//--------------------------------------------------
	void ApiRpcPlugin::Startup()
	{

	}

}



