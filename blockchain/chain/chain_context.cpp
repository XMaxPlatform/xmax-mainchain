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

	ChainContext::~ChainContext()
	{
		if (db_)
		{
			db_->Close();
		}
		db_.reset();
	}

	void ChainContext::Flush()
	{
		db_->Flush();
	}

	void ChainContext::BuildBlock()
	{
		pending_ = std::make_unique<PendingBlock>(db_->StartUndo());

		pending_->PushDB();

		db_->Commit(pending_->undopatch.GetRevision());
		pending_.reset();
	}



	void ChainContext::init()
	{
		//db_.reset(ChainDB::InitDB(fs::current_path(), 1024 * 1024));
		db_.reset(ChainDB::InitDB(config_.DBDir, config_.DBFileSize * MB_SIZE));

		InitSystemTables(getDB());

		Flush();
	}
}
