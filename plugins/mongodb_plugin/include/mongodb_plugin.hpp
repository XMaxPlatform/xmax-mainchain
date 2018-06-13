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
	class MongoDBPlugin : public Xmaxapp::plugin_face {	
		GENERATED_PLUGIN(MongoDBPlugin, Xmaxapp::plugin_face, nullptr)
	public:
		MongoDBPlugin();
		virtual ~MongoDBPlugin();

		virtual void initialize(const Xmaxapp::vars_map& options) override;
		virtual void startup() override;
		virtual void shutdown() override;

	private:
		std::unique_ptr<class MongoDBPluginImpl> impl_;
	};
}
