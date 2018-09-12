/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/dbindexed.hpp>
#include <unitedb/database.hpp>
#include <chaintypes.hpp>

namespace chain
{
	namespace db = unitedb;

	namespace indexed = db::multi_index;

	template<typename T>
	using MVector = db::MVector<T>;

	using MString = db::MString;

	template<typename T>
	using Alloc = db::DBAlloc<T>;

	using DefAlloc = db::DefAlloc;

	using DataPatch = db::DBPatch;

}