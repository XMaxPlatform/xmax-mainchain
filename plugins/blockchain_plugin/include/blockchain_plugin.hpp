/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>

namespace chain
{
	class IChainContext;
}

namespace xmax
{
	using xmaxapp::OptionsDesc;

	class BlockChainPlugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(BlockChainPlugin, xmaxapp::PluginFace, &InitOptions)
	public:

		virtual void Startup() override;

	protected:
		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);

	protected:
		std::unique_ptr<chain::IChainContext> context_;
	};
}