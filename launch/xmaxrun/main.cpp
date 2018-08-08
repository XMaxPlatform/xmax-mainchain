/**
 *  @file
 *  @Copyright (c) 2018 in xmax/LICENSE
 *  @
 */
#include <application.hpp>
#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>
#include <mongodb_plugin.hpp>
#include <xmaxnet_plugin.hpp>
#include <apirpc_plugin.hpp>
#include <pro/protypes.hpp>


namespace xmax
{	
	using namespace xmaxapp;

	///
	//registration function plugin  modules 
	void RegisterPlugins() {
		BlockChainPlugin::RegistSelf();
		BlockBuilderPlugin::RegistSelf();
		MongoDBPlugin::RegistSelf();
		XmaxNetPlugin::RegistSelf();
		ApiRpcPlugin::RegistSelf();
	}

	void InitPlugins(Application& app) {
		app.PluginToInit("BlockChainPlugin");
		app.PluginToInit("BlockBuilderPlugin");
		app.PluginToInit("MongoDBPlugin");
		app.PluginToInit("XmaxNetPlugin");
		app.PluginToInit("ApiRpcPlugin");
	}

	void Run(int argc, char** argv)
	{
		
		// TODO : Add ContractUtilPlugin
		//ContractUtilPlugin::RegistSelf();

		try {
			//PRO_ASSERT(false, "test ${name}", ("name", "xxt"));
			Application app;
			app.SetDefaultConfigFilePath(fs::current_path() / "config" / "config.ini");

			RegisterPlugins();
			InitPlugins(app);

			if (!app.Initialize(argc, argv)) {
				//Logf("Application initialize return false, exit directly.");
				return;
			}

			app.Startup();

			Logf("Xmax app start.");
			app.Loop();
		}
		catch (bpo::error& e) {
			WarnSprintf("Found boost program_option error:%s", e.what());
		}
		catch (std::exception& e) {
			WarnSprintf("Catch exception:%s", e.what());
		}		
		
	}
}

// entry function
int main(int argc, char** argv)
{
	xmax::Run(argc, argv);

	return 0;
}
