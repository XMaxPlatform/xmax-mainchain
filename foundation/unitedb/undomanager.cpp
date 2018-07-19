/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <unitedb/undomanager.hpp>

namespace unitedb
{

	FUndo::UndoID FUndo::scounter_ = 0;

	FUndo::FUndo(UndoManager* owner, UndoRevision rev)
		: owner_(owner)
		, revision_(rev)
	{
		id_ = scounter_++;
	}
	void FUndo::Undo()
	{
		if (valid_)
		{
			owner_->OnUndo(this);
		}
	}
	void FUndo::Cancel()
	{
		if (valid_)
		{
			valid_ = false;
		}

	}

	void FUndo::Combine()
	{
		if (valid_)
		{
			valid_ = false;
			owner_->OnCombine(this);
		}
	}



	void UndoManager::Init()
	{
		std::string type_name = boost::core::demangle( typeid(UndoManager).name() ) + "UndoCache";

		stack_ = owner_->GetMappdFile()->find_or_construct< UndoOpStack >( type_name.c_str() ) (DefAlloc(owner_->GetSegmentManager()) );
	}

	IGenericUndo* UndoManager::StartUndo()
	{
		owner_->EnableUndo(true);
		++stack_->last_undo_;
		FUndo* undo = new FUndo(this, stack_->last_undo_);
		records_.emplace_back(UndoRecord(undo));
		return undo;
	}

	void UndoManager::PushUndo(const UndoOpArg& arg)
	{
		stack_->cache_.EmplaceBack().Set(arg.op_, arg.objid_, arg.table_);
	}

	void UndoManager::LastUpdateFailure(ObjIDCode id)
	{
		BOOST_ASSERT(stack_->cache_.GetBack().id_ == id);
		BOOST_ASSERT(stack_->cache_.GetBack().op_ == UndoOp::Update);

		stack_->cache_.PopBack();
	}

	void UndoManager::OnUndo(FUndo* undo)
	{
		UndoRecord record;
		if (popupRecord(undo->GetID(), record))
		{
			BOOST_ASSERT((stack_->last_commit_ + 1) < record.rev_ && record.rev_ < stack_->last_undo_);
		}
	}

	void UndoManager::OnCombine(FUndo* undo)
	{
		UndoRecord record;
		if (popupRecord(undo->GetID(), record))
		{
			BOOST_ASSERT((stack_->last_commit_ + 1) < record.rev_ && record.rev_ < stack_->last_undo_);
			removeRecords(record.rev_);
		}
	}

	bool UndoManager::popupRecord(FUndo::UndoID id, UndoRecord& out)
	{
		for (auto it = records_.begin(); it != records_.end(); ++it)
		{
			if (it->id_ == id)
			{
				out = *it;
				records_.erase(it);
				return true;
			}
		}

		return false;
	}

	void UndoManager::removeRecords(UndoRevision rev_begin)
	{
		auto it = records_.begin();
		while (it != records_.end())
		{
			if (it->rev_ >= rev_begin)
			{
				it = records_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}