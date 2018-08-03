/**
*  @file
*  @copyright defined in xmax-mainchain/LICENSE
*/
#pragma once
#include <pluginface.hpp>
#include <memory>


namespace xmax
{
	using xmaxapp::OptionsDesc;
	using xmaxapp::VarsMap;
	class XmaxNetPluginImpl;

	/**
	 * This plugin is responsible for the peer to peer communications. Besides it
	 *  also handle the blockchain's data compress and decompress using ProtoBuf.
	 */
	class XmaxNetPlugin : public xmaxapp::PluginFace {	
		GENERATED_PLUGIN(XmaxNetPlugin, xmaxapp::PluginFace, InitOptions)
	public:
		XmaxNetPlugin();
		virtual ~XmaxNetPlugin();

		virtual void Initialize(const xmaxapp::VarsMap& options) override;
		virtual void Startup() override;
		virtual void Shutdown() override;

	protected:

		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);

	private:
		std::shared_ptr<XmaxNetPluginImpl> impl_;
	};
}
