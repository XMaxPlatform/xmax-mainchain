/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <chaintypes.hpp>
#include <chain_db.hpp>
#include <unitedb/dbtable.hpp>
namespace chain
{
	template<typename _Object, typename... _Args>
	using IndexedBy = db::DBIndexedBy<_Object, _Args...>;

	template<typename _multi_index>
	using Table = db::DBTable<_multi_index>;

	class ObjectType
	{
	public:
		enum Code
		{
			NullObject = 0,
			AccountObject,
		};
	};
}