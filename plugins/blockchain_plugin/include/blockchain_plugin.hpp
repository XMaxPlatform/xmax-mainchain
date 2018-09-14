/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>
#include <chain.hpp>


namespace xmax
{
	using xmaxapp::OptionsDesc;
	using xmaxapp::VarsMap;

	class BlockChainPlugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(BlockChainPlugin, xmaxapp::PluginFace, &InitOptions)
	public:

		// xmaxapp::PluginFace interface ----------------------------------
		virtual void Initialize(const VarsMap& options) override;
		virtual void Startup() override;
		virtual void Shutdown() override;

		// ----------------------------------------------------------------


		chain::IChainContext* GetChain() const;
	protected:

		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);

	protected:
		std::unique_ptr<chain::IChainContext> context_;
	};
}