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
	* Serialize blockchain data using mongo db	
	*/
	class MongoDBPlugin : public xmaxapp::PluginFace {	
		GENERATED_PLUGIN(MongoDBPlugin, xmaxapp::PluginFace, nullptr)
	public:
		MongoDBPlugin();
		virtual ~MongoDBPlugin();

		virtual void Initialize(const xmaxapp::VarsMap& options) override;
		virtual void Startup() override;
		virtual void Shutdown() override;

	private:
		std::unique_ptr<class MongoDBPluginImpl> impl_;
	};
}
