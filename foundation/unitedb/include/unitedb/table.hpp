/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtable.hpp>
#include <stack>
namespace unitedb
{

	class ITable
	{
	public:
		virtual ~ITable() {}

		virtual IDBTable* GetDBTable() const = 0;
	};

	class IDatabase
	{
	public:
		virtual ~IDatabase() {}

		virtual mapped_file::segment_manager* GetSegmentManager() const = 0;
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

		std::stack< ObjectType, DBAlloc<ObjectType> > undo_;
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

		}

		virtual void PopUndo() override
		{

		}

	protected:
		UndoCacheType cache_;
		IDatabase * owner_ = nullptr;
	};
}