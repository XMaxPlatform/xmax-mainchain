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

		virtual void BuildBlock() = 0;

		virtual void Flush() = 0;

		static IChainContext* InitContext(ChainConfig& config);
		static void DestroyContext(IChainContext* ptr);

	protected:
		IChainContext();
	};
}