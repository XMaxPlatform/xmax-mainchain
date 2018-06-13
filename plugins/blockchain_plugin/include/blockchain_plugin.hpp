/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace xmax
{
	using xmaxapp::OptionsDesc;

	class BlockChainPlugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(BlockChainPlugin, xmaxapp::PluginFace, &InitOptions)
	public:

	protected:
		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);
	};
}