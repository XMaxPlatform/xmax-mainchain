/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <unitedb/undomanager.hpp>

namespace unitedb
{

	FUndo::UndoID FUndo::scounter_ = 0;

	FUndo::FUndo(UndoManager* owner, DBRevision rev)
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
			valid_ = false;
		}
	}
	void FUndo::Cancel()
	{
		if (valid_)
		{
			owner_->OnCancel(this);
			valid_ = false;
		}

	}

	void FUndo::Combine()
	{
		if (valid_)
		{
			owner_->OnCombine(this);
			valid_ = false;
		}
	}

	bool FUndo::Valid() const
	{
		return valid_;
	}



	void UndoManager::Init()
	{
		std::string stack_name = boost::core::demangle( typeid(UndoManager).name() ) + "UndoCache";

		stack_ = owner_->GetMappdFile()->find_or_construct< UndoOpStack >(stack_name.c_str()) (DefAlloc(owner_->GetSegmentManager()));
	}

	IGenericUndo* UndoManager::StartUndo()
	{
		++stack_->top_revision_;
		DBRevision revision = stack_->top_revision_;
		owner_->OnStartUndo(revision);
		FUndo* undo = new FUndo(this, revision);
		
		getRecords().emplace_back(UndoRecord(undo, stack_->cache_.Size()));
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

	template<typename Func, typename ArrayType, typename IndexType>
	static void ReverseForEach(const ArrayType& data, IndexType rbeg, IndexType rend, Func f)
	{
		for (IndexType i = rbeg; i > rend; --i)
		{
			f(data[i]);
		}
	}

	static UndoRecords::const_iterator findRecord(const UndoRecords& list, FUndo::UndoID id)
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

	static UndoRecords::const_iterator findRecordByRevision(const UndoRecords& list, DBRevision rev)
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			if (it->rev_ == rev)
			{
				return it;
			}
		}
		return list.end();
	}

	static void popExpiredRecords(UndoRecords& list, DBRevision rev_begin)
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


	static void removeCommitedRecords(UndoRecords& list, DBRevision rev_)
	{
		auto it = list.begin();
		while (it != list.end() && it->rev_ <= rev_)
		{
			it = list.erase(it);
		}
	}

	bool UndoManager::Commit(DBRevision rev)
	{
		auto& records = getRecords();
		auto itr = findRecordByRevision(records, rev);
		if (itr != records.end())
		{
			auto next = itr + 1;
			int64_t count = 0;
			if (next != records.end())
			{
				count = next->begin_;
				stack_->cache_.Remove(0, count);
			}
			else
			{
				count = stack_->cache_.Size();
				stack_->cache_.Clear();
			}

			stack_->last_commit_ = rev;
			removeCommitedRecords(records, rev);
			for (auto& itr : records)
			{
				itr.begin_ -= count;
			}

			owner_->OnCommmit(rev);
		}
		return false;
	}

	void UndoManager::OnUndo(FUndo* undo)
	{
		auto itr = findRecord(getRecords(), undo->GetID());

		if (itr != getRecords().end())
		{
			DB_ASSERT(stack_->last_commit_ < itr->rev_ && itr->rev_ <= stack_->top_revision_);
		
			IndexType rbegin = stack_->cache_.Size() - 1;
			IndexType rend = itr->begin_ - 1;

			undoImpl(rbegin, rend);
			popExpiredRecords(getRecords(), itr->rev_);
		}
	}


	void UndoManager::OnCombine(FUndo* undo)
	{
		auto id = undo->GetID();
		auto& records = getRecords();

		for (IndexType i = records.size() - 1; i >= 0; --i)
		{
			auto& curr = records[i];
			if (curr.id_ == id)// find undo info by id.
			{
				DB_ASSERT(stack_->last_commit_ < curr.rev_ && curr.rev_ <= stack_->top_revision_);
				if (0 == i)
				{
					// combine with self, do nothing.
					break;
				}

				records.erase(records.begin() + i);
				owner_->OnCombine(curr.rev_);
			}
		}

		stack_->top_revision_ = records.back().rev_;
	}

	void UndoManager::OnCancel(FUndo* undo)
	{

	}


	void UndoManager::undoImpl(IndexType rbegin, IndexType rend)
	{
		if (rbegin > rend)
		{
			ReverseForEach(stack_->cache_.data_, rbegin, rend, [&](const UndoOp& op) {

				ITable* table = owner_->GetTable(op.table_);
				table->Undo(op);
			});
		}

	}
}