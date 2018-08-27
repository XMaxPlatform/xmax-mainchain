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
		 // default initialize
		ContractUtilPlugin();
		// de construction
		virtual ~ContractUtilPlugin();

		// initialize function
		virtual void Initialize(const xmaxapp::VarsMap& options) override;
		//start up 3rd libarary
		virtual void Startup() override;
		// shut down, clean
		virtual void Shutdown() override;


	private:
		// output logs
		void Test(const char* strlog);
		// TODO : clean 
		void Clean();

	private:
		// code implement
		std::unique_ptr<class ContractUtilPluginImpl> impl_;
	};
}
