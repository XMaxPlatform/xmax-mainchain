/**
*  @file
*  @copyright defined in xmax-mainchain/LICENSE
*/
#pragma once
#include <pluginface.hpp>
#include <memory>

namespace Xmax
{

	/**
	* Serialize blockchain data using mongo db	
	*/
	class mongodb_plugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(mongodb_plugin, xmaxapp::PluginFace, nullptr)
	public:
		mongodb_plugin();
		virtual ~mongodb_plugin();

		virtual void initialize(const xmaxapp::vars_map& options) override;
		virtual void startup() override;
		virtual void shutdown() override;

	private:
		std::unique_ptr<class mongodb_plugin_impl> m_pImpl;
	};
}
