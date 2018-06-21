/**
*  @file
*  @copyright defined in xmax-mainchain/LICENSE
*/
#pragma once
#include <pluginface.hpp>
#include <memory>


namespace xmax
{

	/**
	 * Contract util plugin
	 */
	class ContractUtilPlugin : public xmaxapp::PluginFace {	
		GENERATED_PLUGIN(ContractUtilPlugin, xmaxapp::PluginFace, nullptr)
	public:
		ContractUtilPlugin();
		virtual ~ContractUtilPlugin();

}
