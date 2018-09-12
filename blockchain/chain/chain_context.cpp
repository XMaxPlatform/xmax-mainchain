/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <chain_context.hpp>
#include <chain_tables.hpp>

namespace chain
{

	ChainContext::ChainContext(const ChainConfig& config)
		: config_(config)
	{
		init();
	}
	ChainContext::ChainContext(ChainConfig&& config)
		: config_(std::move(config))
	{
		init();
	}

	void ChainContext::BuildBlock()
	{
		//pending_ = std::make_unique<PendingBlock>(db_->StartUndo());

		//pending_->PushDB();
	}

	void ChainContext::init()
	{
		db_.reset(ChainDB::InitDB(config_.DBDir, config_.DBFileSize * MB_SIZE));

		InitSystemTables(getDB());
	}
}
