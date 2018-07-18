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
			, revision(0)
			, opcode(UndoOp::None)
		{

		}

		void Set(const ObjectType& data, UndoRevision v, UndoOp::UndoCode c)
		{
			*static_cast<Super*>(this) = data;
			revision = v;
			opcode = c;
		}

		inline UndoOp::UndoCode GetUndoCode() const
		{
			return opcode;
		}

	protected:
		UndoOp::UndoCode opcode;
		UndoRevision revision;
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
		typedef MappedUndo<UndoObjectType> UndoCacheType;

		typedef typename DBTableType Super;

		FTable(IDatabase* owner, MappedPtr p)
			: owner_(owner)
			, Super(p)
		{
			std::string type_name = boost::core::demangle(typeid(SelfType).name()) + "UndoCache";

			cache_ = owner_->GetMappdFile()->find_or_construct< UndoCacheType >(type_name.c_str()) (UndoCacheType::AllocType(owner_->GetSegmentManager()));
		}

		inline const ObjectType* AsObject(const DBObjectBase* base)
		{
			return static_cast<const ObjectType*>(base);
		}

		virtual IDBTable* GetDBTable() const override
		{
			return const_cast<SelfType*>(this);
		}

		virtual void PushUndo(UndoOp::UndoCode code, const DBObjectBase* undo) override
		{
			if (noUndo())
			{
				return;
			}

			switch (code)
			{
			case unitedb::UndoOp::None:
				break;
			case unitedb::UndoOp::Create:
			{
				owner_->PushUndo( UndoOpArg(code, DBObjectBase::__getObjidcode(*undo)) );
			}
			break;
			case unitedb::UndoOp::Update:
			case unitedb::UndoOp::Delete:
			{
				PushUndoObject(undo, owner_->TopRevision(), code);
				owner_->PushUndo( UndoOpArg(code, DBObjectBase::__getObjidcode(*undo)) );
			}
			break;
			default:
				break;
			}
		}

		virtual void LastUpdateFailure() override
		{
			if (noUndo())
			{
				//PRO_STATIC_ASSERT()
				return;
			}
		}

		virtual void SetUndo(bool set)
		{
			no_undo_ = !set;
		}

	protected:

		inline bool noUndo() const
		{
			return no_undo_;
		}

		void PushUndoObject(const DBObjectBase* undo, UndoRevision v, UndoOp::UndoCode c)
		{
			cache_->EmplaceBack().Set(*AsObject(undo), v, c);
		}

		bool no_undo_ = true;
		UndoCacheType* cache_;
		IDatabase * owner_ = nullptr;
	};
}