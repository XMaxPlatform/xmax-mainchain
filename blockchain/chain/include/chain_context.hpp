/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <chaintypes.hpp>
#include <chain.hpp>
#include <pending_block.hpp>

namespace chain
{
	using ChainDB = db::Database;

	class ChainContext : public IChainContext
	{
	public:
		ChainContext(const ChainConfig& config);
		ChainContext(ChainConfig&& config);
		~ChainContext();

		virtual void BuildBlock();

		virtual void Flush();

	protected:
		void init();

		ChainDB& getDB();
		const ChainDB& getDB() const;

		ChainConfig config_;
		std::unique_ptr<ChainDB> db_;
		std::unique_ptr<PendingBlock> pending_;
	};


	inline ChainDB& ChainContext::getDB()
	{
		return *db_;
	}
	inline const ChainDB& ChainContext::getDB() const
	{
		return *db_;
	}
}