/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtable.hpp>
#include <unitedb/dbundomanager.hpp>
#include <deque>
namespace unitedb
{

	class ITable
	{
	public:
		virtual ~ITable() {}

		virtual IDBTable* GetDBTable() const = 0;

		virtual void SetUndo(bool set) = 0;
	};

	class IDatabase
	{
	public:
		virtual ~IDatabase() {}

		virtual mapped_file::segment_manager* GetSegmentManager() const = 0;

		virtual void PushUndo(const UndoOpArg& arg) = 0;

		virtual void PopUndo() = 0;
	};

	template<typename T>
	class FUndoCache
	{
	public:
		typedef T ObjectType;

		template<typename Alloc>
		FUndoCache(const Alloc& cc)
			: undo_(cc)
		{

		}

		std::deque< ObjectType, DBAlloc<ObjectType> > undo_;
	};


	template<typename T>
	class FTable : public T,  public ITable
	{
	public:
		typedef FUndoCache<T> UndoCacheType;
		typedef typename T DBTableType;
		typedef typename DBTableType Super;
		typedef typename FTable<T> SelfType;
		typedef typename T::MappedPtr MappedPtr;
		FTable(IDatabase* owner, MappedPtr p)
			: owner_(owner)
			, cache_(owner->GetSegmentManager())
			, Super(p)
		{
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
			no_undo_ = set;
		}

	protected:

		inline bool noUndo() const
		{
			return no_undo_;
		}

		bool no_undo_ = true;
		UndoCacheType cache_;
		IDatabase * owner_ = nullptr;
	};
}