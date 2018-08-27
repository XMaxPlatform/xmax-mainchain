/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <chain.hpp>
#include <chain_context.hpp>

namespace chain
{
	IChainContext* IChainContext::InitContext(ChainConfig& config)
	{
		return new ChainContext(config);
	}

	void IChainContext::DestroyContext(IChainContext* ptr)
	{
		delete ptr;
	}


	IChainContext::IChainContext()
	{

	}
}
