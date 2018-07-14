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
	class DBTableInst : public T
	{
	public: 
		typedef typename T::MappedPtr MappedPtr;
		DBTableInst(MappedPtr p)
			: T(p)
		{

		}
	};

	template<typename T>
	class FTable : public ITable
	{
	public:
		typedef typename DBTableInst<T> DBTableType;

		typedef typename T::MappedPtr MappedPtr;
		FTable(IDatabase* owner, MappedPtr p)
			: owner_(owner)
			, table(p)
		{
		}

		virtual IDBTable* GetDBTable() const override
		{
			return const_cast<DBTableType*>(&table);
		}

	protected:
		DBTableType table;
		IDatabase * owner_ = nullptr;
	};
}