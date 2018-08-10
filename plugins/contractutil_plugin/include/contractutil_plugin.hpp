/**
*  @file
*  @copyright defined in xmax-mainchain/LICENSE
*/
#pragma once
#include <pluginface.hpp>
#include <memory>


namespace xmax
{

	/**
	 * Contract util plugin
	 */
	class ContractUtilPlugin : public xmaxapp::PluginFace {	
		GENERATED_PLUGIN(ContractUtilPlugin, xmaxapp::PluginFace, nullptr)
	public:
		ContractUtilPlugin();
		virtual ~ContractUtilPlugin();

		// initialize function
		virtual void Initialize(const xmaxapp::VarsMap& options) override;
		virtual void Startup() override;
		virtual void Shutdown() override;

	private:
		// output logs
		void Test(const char* strlog);

	private:
		std::unique_ptr<class ContractUtilPluginImpl> impl_;
	};
}
