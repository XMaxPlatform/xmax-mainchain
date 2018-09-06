/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>
#include <blockchain_plugin.hpp>

namespace xmax
{
	using xmaxapp::OptionsDesc;
	using xmaxapp::VarsMap;

	class BlockBuilderImpl;


/*!
 * \class BlockBuilderPlugin
 * \brief The builder plugin use for build logic.
 */
	class BlockBuilderPlugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(BlockBuilderPlugin, xmaxapp::PluginFace, &InitOptions)
		PLUGIN_DEPENDS(BlockChainPlugin)
	public:
		BlockBuilderPlugin();
		// xmaxapp::PluginFace interface ----------------------------------
		virtual void Initialize(const VarsMap& options) override;
		virtual void Startup() override;
			
		// ----------------------------------------------------------------
	protected:

		virtual void OnCreated() override;

		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);		
		


		std::unique_ptr<BlockBuilderImpl> impl_;
	};
}
