/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace Xmax
{
	using xmaxapp::OptionsDesc;
	class blockbuilder_plugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(blockbuilder_plugin, xmaxapp::PluginFace, &InitOptions)
	public:

		virtual void Startup() override;

	protected:
		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);

	};
}
