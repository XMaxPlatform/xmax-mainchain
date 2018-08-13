/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtable.hpp>
#include <unitedb/undo.hpp>
namespace unitedb
{
	template<typename T>
	struct UndoObject : protected T
	{
	public:
		typedef T ObjectType;
		typedef T Super;
		typedef DBAlloc< ObjectType > AllocType;

		UndoObject(const AllocType& cc)
			: Super(cc)
			, revision_(0)
			, opcode_(UndoOp::None)
		{

		}

		void Set(const ObjectType& data, DBRevision v, UndoOp::UndoCode c)
		{
			*static_cast<Super*>(this) = data;
			revision_ = v;
			opcode_ = c;
		}

		const ObjectType& Object() const
		{
			return *((ObjectType*)this);
		}


		inline UndoOp::UndoCode GetUndoCode() const
		{
			return opcode_;
		}
		inline ObjIDCode ObjID() const
		{
			return DBObjBase::__getObjidcode(*this);
		}

	protected:
		UndoOp::UndoCode opcode_;
		DBRevision revision_;
	};

	struct TableUndoInfo
	{
		typedef int32_t IndexType;

		TableUndoInfo(IndexType beg, DBRevision rev)
			: begin_(beg)
			, rev_(rev)
		{

		}

		IndexType begin_ = 0;
		DBRevision rev_ = InvalidRevision;
	};

	typedef MVector<TableUndoInfo> TableUndoInfos;

	template<typename T>
	class TableUndoStack
	{
	public:
		typedef T UndoObjectType;
		typedef MappedUndo<UndoObjectType> UndoCacheType;

		TableUndoStack(DefAlloc al)
			: cache_(al)
			, infos_(al)
		{

		}

		void Reset()
		{
			cache_.Clear();
			infos_.clear();
		}

		UndoCacheType cache_;

		TableUndoInfos infos_;
	};


	template<typename T>
	class FTable : public T,  public ITable
	{
	public:
		typedef typename T DBTableType;
		typedef typename DBTableType::ObjectType ObjectType;
		typedef typename FTable<DBTableType> SelfType;
		typedef typename DBTableType::MappedPtr MappedPtr;

		typedef UndoObject<ObjectType> UndoObjectType;
		typedef TableUndoStack<UndoObjectType> UndoStackType;

		typedef typename DBTableType Super;

		FTable(IDatabase* owner, MappedPtr p)
			: owner_(owner)
			, Super(p)
		{
			std::string type_name = boost::core::demangle(typeid(SelfType).name()) + "UndoCache";

			stack_ = owner_->GetMappdFile()->find_or_construct< UndoStackType >(type_name.c_str()) (DefAlloc(owner_->GetSegmentManager()));
		}

		inline const ObjectType* AsObject(const DBObjBase* base)
		{
			return static_cast<const ObjectType*>(base);
		}

		virtual IDBTable* GetDBTable() const override
		{
			return const_cast<SelfType*>(this);
		}

		virtual void PushUndo(UndoOp::UndoCode code, const DBObjBase* undo) override
		{
			pushUndoImpl(code, undo);
		}

		virtual void LastUpdateFailure(ObjIDCode id) override
		{
			if (noUndo())
			{
				DB_ASSERT(stack_->cache_.GetBack().GetUndoCode() == UndoOp::Update);
				if (stack_->cache_.GetBack().ObjID() == id)
				{
					stack_->cache_.PopBack();
				}
				owner_->LastUpdateFailure(id);
				return;
			}
		}

		virtual void StartUndo(DBRevision revision) override
		{
			stack_->infos_.emplace_back( TableUndoInfo(stack_->cache_.Size(), revision) );
			no_undo_ = false;
		}

		virtual void Combine(DBRevision revision) override
		{
			// note, when combine with self, do nothing.so "i > 0".

			for (int i = stack_->infos_.size() - 1; i > 0; --i)
			{
				if (stack_->infos_[i].rev_ == revision)
				{
					stack_->infos_.erase(stack_->infos_.begin() + i);
					break;
				}
			}
		}

		virtual void Commit(DBRevision revision) override
		{
			int idx = -1;
			for (int i = 0; i < stack_->infos_.size(); ++i)
			{
				if (stack_->infos_[i].rev_ == revision)
				{
					idx = i;
					break;
				}
			}
			if (idx == -1) // never be true.
			{
				stack_->Reset();
				return;
			}
			// remove cache data.

			const TableUndoInfo& revinfo = stack_->infos_[idx];

			const int remcount = revinfo.begin_;

			stack_->cache_.Remove(0, remcount);

			// remove info data.
			int fixcount = idx + 1;
			auto remend = stack_->infos_.begin() + fixcount;

			stack_->infos_.erase(stack_->infos_.begin(), remend);

			for (auto& itr : stack_->infos_)
			{
				itr.begin_ -= fixcount;
			}

		}

		virtual void Undo(const UndoOp& op) override
		{
			undoImpl(op);
		}

	protected:

		inline bool noUndo() const
		{
			return no_undo_;
		}

		void pushUndoImpl(UndoOp::UndoCode code, const DBObjBase* undo)
		{
			if (noUndo())
			{
				return;
			}

			switch (code)
			{
			case unitedb::UndoOp::Create:
			{
				owner_->PushUndo(UndoOpArg(code, DBObjBase::__getObjidcode(*undo), ObjectType::TypeCode));
			}
			break;
			case unitedb::UndoOp::Update:
			case unitedb::UndoOp::Delete:
			{
				pushUndoObject(undo, owner_->TopRevision(), code);
				owner_->PushUndo(UndoOpArg(code, DBObjBase::__getObjidcode(*undo), ObjectType::TypeCode));
			}
			break;
			default:
				break;
			}
		}
		void undoImpl(const UndoOp& op)
		{
			switch (op.op_)
			{
			case UndoOp::Create:
			{
				auto itr = GetMapped().find(op.id_);
				DB_ASSERT(itr != GetMapped().end());
				GetMapped().erase(itr);
			}
			break;
			case UndoOp::Update:
			{
				auto itr = GetMapped().find(op.id_);
				DB_ASSERT(itr != GetMapped().end());
				const auto& bak = getBackObject();
				GetMapped().replace(itr, bak);
				stack_->cache_.PopBack();
			}
			break;
			case UndoOp::Delete:
			{
				GetMapped().emplace(getBackObject());
				stack_->cache_.PopBack();
			}
			break;

			default:
				break;
			}
		}

		void pushUndoObject(const DBObjBase* undo, DBRevision v, UndoOp::UndoCode c)
		{
			stack_->cache_.EmplaceBack().Set(*AsObject(undo), v, c);
		}

		const ObjectType& getBackObject() const
		{
			return stack_->cache_.GetBack().Object();
		}

		bool no_undo_ = true;
		UndoStackType* stack_;
		IDatabase * owner_ = nullptr;
	};
}