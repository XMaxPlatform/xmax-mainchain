/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include "blockbuilder_plugin.hpp"

namespace Xmax
{

	void blockbuilder_plugin::Startup()
	{
		Super::Startup();
	}

	void blockbuilder_plugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		ilog("Blockbuilder plugin init options.");
	}

}