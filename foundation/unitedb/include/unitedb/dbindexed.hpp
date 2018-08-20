/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/ordered_index.hpp> 
#include <boost/multi_index/member.hpp>

#include <unitedb/dbobjectbase.hpp>

namespace unitedb
{
	namespace multi_index = boost::multi_index;

	struct ByObjectID;

#define INDEXED_BY_OBJECT_ID multi_index::ordered_unique<boost::multi_index::tag<ByObjectID>, boost::multi_index::member<DBObjectBase, ObjIDCode, &DBObjectBase::__objid>>

	template<typename _Object, typename... _Args>
	using DBTableDeclaration = boost::multi_index_container<_Object, _Args..., DBAlloc<_Object> >;


	template<typename _Object, typename... _Args>
	using DBIndexedBy = boost::multi_index_container<
		_Object,
		boost::multi_index::indexed_by<
		INDEXED_BY_OBJECT_ID,
		_Args...
		>,
		DBAlloc<_Object>
	>;

}
