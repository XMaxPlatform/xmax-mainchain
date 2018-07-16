/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <deque>
namespace unitedb
{
	template<typename T>
	class FUndoCache
	{
	public:
		typedef T ObjectType;
		typedef DBAlloc< FUndoCache<T> > AllocType;
		FUndoCache(const AllocType& cc)
			: undo_(cc)
		{

		}

		std::deque< ObjectType, DBAlloc<ObjectType> > undo_;
	};

}