/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <chain.hpp>
#include <blockchain_plugin.hpp>


namespace xmax
{
	using namespace chain;

	void BlockChainPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{

	}


	void BlockChainPlugin::Startup()
	{
		IChainContext* c = IChainContext::InitContext(ChainConfig());
		context_.reset(c);
	}


	IChainContext* BlockChainPlugin::GetChain() const
	{
		return context_.get();
	}
}