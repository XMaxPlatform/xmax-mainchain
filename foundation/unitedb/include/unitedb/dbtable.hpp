/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/dbobject.hpp>

namespace unitedb
{
	class Database;
	class ITable
	{
	public:
		virtual ~ITable() {}
	};

	class DBTableBase : public ITable
	{

	public:
		ObjectIDCode GenerateID()
		{
			++counter_;
			return counter_;
		}

	private:
		ObjectIDCode counter_ = 0;
	};

	template<typename _multi_index>
	class DBTable : public DBTableBase
	{
	public:
		typedef _multi_index MultiIndexType;
		typedef typename MultiIndexType::value_type ObjectType;
		typedef DBAlloc<DBTable> AllocType;

		static std::string TableName()
		{
			static std::string type_name = boost::core::demangle(typeid(typename ObjectType).name()) + "Table";
			return type_name;
		}

		DBTable(DBAlloc<ObjectType> alloc)
			: indices_(alloc)
		{

		}

		friend class Database;

	protected:
		MultiIndexType indices_;

	};

	struct ByID;

	#define INDEXED_BY_ID boost::multi_index::ordered_unique<boost::multi_index::tag<ByID>, boost::multi_index::member<DBObjectBase, ObjectIDCode, &DBObjectBase::id_>>

	template<typename _Object, typename... _Args>
	using DBTableDeclaration = boost::multi_index_container<_Object, _Args..., DBAlloc<_Object> >;


}