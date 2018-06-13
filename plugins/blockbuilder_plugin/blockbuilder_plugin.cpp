/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include "blockbuilder_plugin.hpp"

namespace xmax
{

	void BlockBuilderPlugin::Startup()
	{
		Super::Startup();
	}

	void BlockBuilderPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		ilog("Blockbuilder plugin init options.");
	}

}