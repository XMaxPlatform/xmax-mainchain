/**
 *  @file
 *  @Copyright (c) 2018 in xmax/LICENSE
 *  @
 */
#include <application.hpp>
#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>
#include <pro/log/log.hpp>

namespace Xmax
{

	
	using namespace xmaxapp;
	void Run(int argc, char** argv)
	{
		blockchain_plugin::RegistSelf();
		blockbuilder_plugin::RegistSelf();

		Application app;

		app.PluginToInit("blockchain_plugin");
		app.PluginToInit("blockbuilder_plugin");

		app.Initialize(argc, argv);
		app.Startup();

		ilog("Xmax app start.");
		app.Loop();
	}
}

// entry function
int main(int argc, char** argv)
{
	Xmax::Run(argc, argv);
	return 0;
}
