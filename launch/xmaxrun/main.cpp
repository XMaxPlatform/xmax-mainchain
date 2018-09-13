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

#ifdef NO_CATCH
#define SAVE_EXIT 0
#else
#define SAVE_EXIT 1
#endif 

namespace xmax
{	
	using namespace xmaxapp;

	///
	//registration function plugin  modules 
	static void RegisterPlugins(Application& app) {

		app.RegisterPlugin<BlockChainPlugin>(true);
		app.RegisterPlugin<BlockBuilderPlugin>(true);
		app.RegisterPlugin<MongoDBPlugin>();
		app.RegisterPlugin<XmaxNetPlugin>();
		app.RegisterPlugin<ApiRpcPlugin>();
	}

	static void Run(int argc, char** argv)
	{
		
		// TODO : Add ContractUtilPlugin
		//ContractUtilPlugin::RegistSelf();
		Application app;
#if SAVE_EXIT
		try
#endif
		{
			//PRO_ASSERT(false, "test ${name}", ("name", "xxt"));
	
			app.SetDefaultConfigFilePath(fs::current_path() / "config" / "config.ini");

			RegisterPlugins(app);

			if (!app.Initialize(argc, argv)) {
				//Logf("Application initialize return false, exit directly.");
				return;
			}

			app.Startup();

			Logf("Xmax app start.");
			app.Loop();
		}
#if SAVE_EXIT
		catch (bpo::error& e) {
			WarnSprintf("Found boost program_option error:%s", e.what());
		}
		catch (std::exception& e) {
			WarnSprintf("Catch exception:%s", e.what());
		}		
#endif	
		app.Quit();
	}

}

// entry function
int main(int argc, char** argv)
{
	xmax::Run(argc, argv);

	return 0;
}
