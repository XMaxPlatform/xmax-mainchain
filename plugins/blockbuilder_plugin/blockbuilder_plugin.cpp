/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include "blockbuilder_plugin.hpp"

namespace Xmax
{

	void blockbuilder_plugin::startup()
	{
		super::startup();
	}

	void blockbuilder_plugin::init_options(options_desc& cli, options_desc& cfg)
	{
		ilog("Blockbuilder plugin init options.");
	}

}