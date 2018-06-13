/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace xmax
{
	using xmaxapp::OptionsDesc;
	class BlockBuilderPlugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(BlockBuilderPlugin, xmaxapp::PluginFace, &InitOptions)
	public:

		virtual void Startup() override;

	protected:
		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);

	};
}
