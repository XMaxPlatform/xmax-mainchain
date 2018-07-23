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
		FUndo* undo = new FUndo(this, stack_->undo_counter_);
		++stack_->undo_counter_;
		records_.emplace_back(UndoRecord(undo, (UndoRecord::IndexType)stack_->Size()));
		return undo;
	}

	void UndoManager::PushUndo(const UndoOpArg& arg)
	{
		stack_->cache_.EmplaceBack().Set(arg.op_, arg.objid_, arg.table_);
	}

	void UndoManager::LastUpdateFailure(ObjIDCode id)
	{
		DB_ASSERT(stack_->cache_.GetBack().id_ == id);
		DB_ASSERT(stack_->cache_.GetBack().op_ == UndoOp::Update);

		stack_->cache_.PopBack();
	}

	using Records = UndoManager::UndoRecords;

	template<typename Func, typename IndexType>
	static void reverseForEach(const UndoOpStack& stack, IndexType rbeg, IndexType rend, Func f)
	{
		using ArrayType = UndoOpStack::StackType::ArrayType;

		const ArrayType& data = stack.cache_.data_;

		for (IndexType i = rbeg; i > rend; --i)
		{
			f(data[i]);
		}
	}

	static Records::const_iterator findRecord(const Records& list, FUndo::UndoID id)
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			if (it->id_ == id)
			{
				return it;
			}
		}
		return list.end();
	}
	static void removeExpiredRecords(Records& list, UndoRevision rev_begin)
	{
		auto it = list.begin();
		while (it != list.end())
		{
			if (it->rev_ >= rev_begin)
			{
				it = list.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void UndoManager::OnUndo(FUndo* undo)
	{
		auto it = findRecord(records_, undo->GetID());

		if (it != records_.end())
		{
			DB_ASSERT(stack_->last_commit_ < it->rev_ && it->rev_ < stack_->undo_counter_);
		
			int64_t rbegin = stack_->Size() - 1;
			int64_t rend = it->begin_ - 1;

			undoImpl(rbegin, rend);
			removeExpiredRecords(records_, it->rev_);
		}
	}


	void UndoManager::OnCombine(FUndo* undo)
	{
		UndoRecord record;
		if (popupRecord(undo->GetID(), record))
		{
			DB_ASSERT((stack_->last_commit_ + 1) < record.rev_ && record.rev_ < stack_->undo_counter_);
		}
	}


	void UndoManager::undoImpl(int64_t rbegin, int64_t rend)
	{
		if (rbegin > rend)
		{
			reverseForEach(*stack_, rbegin, rend, [&](const UndoOp& op) {

				ITable* table = owner_->GetTable(op.table_);
				table->Undo(op);
			});
		}

	}

	bool UndoManager::popupRecord(FUndo::UndoID id, UndoRecord& out) // get a record and remove it.
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
}