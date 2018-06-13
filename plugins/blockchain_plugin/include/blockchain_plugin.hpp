/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace Xmax
{
	using xmaxapp::OptionsDesc;

	class blockchain_plugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(blockchain_plugin, xmaxapp::PluginFace, &init_options)
	public:

	protected:
		static void init_options(OptionsDesc& cli, OptionsDesc& cfg);
	};
}