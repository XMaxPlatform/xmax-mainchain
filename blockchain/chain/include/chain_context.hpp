/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <chaintypes.hpp>
#include <chain.hpp>
#include <unitedb/database.hpp>

namespace chain
{
	class ChainContext : public IChainContext
	{
	public:
		ChainContext(const ChainConfig& config);

	protected:
	};
}