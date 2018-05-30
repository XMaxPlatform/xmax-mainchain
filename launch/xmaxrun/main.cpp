/**
 *  @file
 *  @copyright defined in xmax/LICENSE
 */
#include <application.hpp>
#include <blockchain_plugin.hpp>
#include <log.hpp>

namespace Xmax
{

	void run(int argc, char** argv)
	{
		application app;
		app.regist_plugin<blockchain_plugin>();
		app.initialize(argc, argv);
		app.startup();

		ilog("Xmax app start.");
		app.exec();
	}
}

int main(int argc, char** argv)
{
	Xmax::run(argc, argv);
	return 0;
}
