/**
 *  @file
 *  @Copyright (c) 2018 in xmax/LICENSE
 *  @
 */
#include <application.hpp>
#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>
#include <pro/log/log.hpp>

namespace xmax
{

	
	using namespace xmaxapp;
	void Run(int argc, char** argv)
	{
		BlockChainPlugin::RegistSelf();
		BlockBuilderPlugin::RegistSelf();

		Application app;

		app.PluginToInit("BlockChainPlugin");
		app.PluginToInit("BlockBuilderPlugin");

		app.Initialize(argc, argv);
		app.Startup();

		ilog("Xmax app start.");
		app.Loop();
	}
}

// entry function
int main(int argc, char** argv)
{
	xmax::Run(argc, argv);
	return 0;
}
