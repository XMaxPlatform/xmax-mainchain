/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <chaintypes.hpp>

namespace chain
{

	class ChainConfig
	{
	public:

	};


	class IChainContext
	{
	public:
		virtual ~IChainContext() {};

		static IChainContext* InitContext(ChainConfig& config);
		static void DestroyContext(IChainContext* ptr);

	protected:
		IChainContext();
	};
}