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
#include <pro/log/log.hpp>


namespace xmax
{	
	using namespace xmaxapp;


	void RegisterPlugins() {
		BlockChainPlugin::RegistSelf();
		BlockBuilderPlugin::RegistSelf();
		MongoDBPlugin::RegistSelf();
		XmaxNetPlugin::RegistSelf();
	}

	void InitPlugins(Application& app) {
		app.PluginToInit("BlockChainPlugin");
		app.PluginToInit("BlockBuilderPlugin");
		app.PluginToInit("MongoDBPlugin");
		app.PluginToInit("XmaxNetPlugin");
	}

	void Run(int argc, char** argv)
	{
		
		// TODO : Add ContractUtilPlugin
		//ContractUtilPlugin::RegistSelf();

		Application app;
		app.SetDefaultConfigFilePath(fs::current_path() / "config" / "config.ini");

		RegisterPlugins();
		InitPlugins(app);
		
		app.Initialize(argc, argv);
		app.Startup();

		LogSprintf("Xmax app start.");
		app.Loop();
	}
}

// entry function
int main(int argc, char** argv)
{
	xmax::Run(argc, argv);
	return 0;
}
