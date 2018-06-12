/**
*  @file
*  @copyright defined in xmax-mainchain/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace Xmax
{
	class xmaxnet_plugin : public plugin_face
	{
		GENERATED_PLUGIN(xmaxnet_plugin, plugin_face)
	public:
		xmaxnet_plugin();
		virtual ~xmaxnet_plugin();

		virtual void initialize(const variables_map& options) override;
		virtual void startup() override;
	};
}
