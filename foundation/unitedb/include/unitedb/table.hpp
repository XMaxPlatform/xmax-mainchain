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
	struct UndoObject : public T
	{
	public:
	public:
		typedef T ObjectType;
		typedef T Super;
		typedef DBAlloc< ObjectType > AllocType;
		UndoObject(const AllocType& cc, UndoRevision v)
			: Super(cc)
			, revision(v)
		{

		}

		UndoRevision revision;
	};

	template<typename T>
	class FTable : public T,  public ITable
	{
	public:
		typedef typename T DBTableType;
		typedef typename FTable<DBTableType> SelfType;
		typedef typename DBTableType::MappedPtr MappedPtr;

		typedef UndoObject<DBTableType> UndoObjectType;
		typedef MappedUndo<UndoObjectType> UndoCacheType;

		typedef typename DBTableType Super;

		FTable(IDatabase* owner, MappedPtr p)
			: owner_(owner)
			, Super(p)
		{
			std::string type_name = boost::core::demangle(typeid(SelfType).name()) + "UndoCache";

			cache_ = owner_->GetMappdFile()->find_or_construct< UndoCacheType >(type_name.c_str()) (UndoCacheType::AllocType(owner_->GetSegmentManager()));
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

			}
				break;
			case unitedb::UndoOp::Update:
			{

			}
				break;
			case unitedb::UndoOp::Delete:
			{

			}
				break;
			default:
				break;
			}
		}

		virtual void PopUndo() override
		{
			if (noUndo())
			{
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

		bool no_undo_ = true;
		UndoCacheType* cache_;
		IDatabase * owner_ = nullptr;
	};
}