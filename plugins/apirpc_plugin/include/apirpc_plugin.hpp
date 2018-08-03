/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace xmax
{
	using xmaxapp::OptionsDesc;
	using xmaxapp::VarsMap;
	
	class ApiRpcPluginImpl;

/*!
 * \class ApiRPCPlugin
 * \brief The builder plugin use for build logic.
 */
	class ApiRpcPlugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(ApiRpcPlugin, xmaxapp::PluginFace, &InitOptions)
	public:
		ApiRpcPlugin();
		// xmaxapp::PluginFace interface ----------------------------------
		virtual void Initialize(const VarsMap& options) override;
		virtual void Startup() override;
			
		// ----------------------------------------------------------------
	protected:


		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);		
		


		std::unique_ptr<ApiRpcPluginImpl> impl_;
	};
}
