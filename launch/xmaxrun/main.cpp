/**
 *  @file
 *  @copyright defined in xmax/LICENSE
 */
#include <application.hpp>
#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>
#include <pro/log/log.hpp>

namespace Xmax
{
	using namespace Xmaxapp;
	void run(int argc, char** argv)
	{
		application app;
		app.regist_plugin<blockchain_plugin>();
		app.regist_plugin<blockbuilder_plugin>();
		app.initialize(argc, argv);
		app.startup();

		ilog("Xmax app start.");
		app.loop();
	}
}

int main(int argc, char** argv)
{
	Xmax::run(argc, argv);
	return 0;
}
