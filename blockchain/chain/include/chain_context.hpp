/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <chaintypes.hpp>
#include <chain.hpp>

namespace chain
{
	class ChainContext : public IChainContext
	{
	public:
		ChainContext(const ChainConfig& config);
	};
}