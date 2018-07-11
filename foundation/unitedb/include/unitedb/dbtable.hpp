/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/dbobject.hpp>

namespace unitedb
{

	class ITable
	{
	public:
		virtual ~ITable() {}
	};

	template<typename _multi_index>
	class DBTable : public ITable
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

	private:
		MultiIndexType indices_;

	};


	template<typename _Object, typename... _Args>
	using DBTableDeclaration = boost::multi_index_container<_Object, _Args..., DBAlloc<_Object> >;


}