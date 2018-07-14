/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtable.hpp>
namespace unitedb
{

	class ITable
	{
	public:
		virtual ~ITable() {}

		virtual IDBTable* GetDBTable() const = 0;
	};

	template<typename T>
	class FTable : public T,  public ITable
	{
	public:
		typedef typename T DBTableType;
		typedef typename DBTableType Super;
		typedef typename FTable<T> SelfType;
		typedef typename T::MappedPtr MappedPtr;
		FTable(IDatabase* owner, MappedPtr p)
			: owner_(owner)
			, Super(p)
		{
		}

		virtual IDBTable* GetDBTable() const override
		{
			return const_cast<SelfType*>(this);
		}
	protected:
		IDatabase * owner_ = nullptr;
	};
}