/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <chain.hpp>
#include <blockchain_plugin.hpp>


namespace xmax
{
	using namespace chain;
	using namespace pro;

	void BlockChainPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		cfg.add_options()
			("db-runtime-size", xmaxapp::options::value<uint64_t>()->default_value(1024), "size(in mb) of db memory.");
	}

	void BlockChainPlugin::Initialize(const VarsMap& options)
	{
		Super::Initialize(options);

		ChainConfig cfg;
		cfg.DBDir = GetApp()->GetDataDir() / "db";
		cfg.DBFileSize = options.at("db-runtime-size").as<uint64_t>();

		IChainContext* c = IChainContext::InitContext(cfg);
		context_.reset(c);
	}

	void BlockChainPlugin::Startup()
	{
		Super::Startup();
	}

	void BlockChainPlugin::Shutdown()
	{
		context_->Flush();
		context_.reset();
		Super::Shutdown();

	}


	IChainContext* BlockChainPlugin::GetChain() const
	{
		return context_.get();
	}
}