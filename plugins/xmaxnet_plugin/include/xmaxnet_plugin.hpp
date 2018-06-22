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
	 * This plugin is responsible for the peer to peer communications. Besides it
	 *  also handle the blockchain's data compress and decompress using ProtoBuf.
	 */
	class XmaxNetPlugin : public xmaxapp::PluginFace {	
		GENERATED_PLUGIN(XmaxNetPlugin, xmaxapp::PluginFace, nullptr)
	public:
		XmaxNetPlugin();
		virtual ~XmaxNetPlugin();

		virtual void Initialize(const xmaxapp::VarsMap& options) override;
		virtual void Startup() override;
		virtual void Shutdown() override;

	public:

		void Connect(const std::string& endPoint);

	private:
		std::unique_ptr<class XmaxNetPluginImpl> impl_;
	};
}
