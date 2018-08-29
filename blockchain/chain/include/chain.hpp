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
		fs::path DBDir;
		uint64_t DBFileSize = MB(256);
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