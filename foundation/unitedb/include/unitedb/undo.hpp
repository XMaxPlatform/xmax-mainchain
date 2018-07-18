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
	class MappedUndo
	{
	public:
		typedef T ObjectType;
		typedef DBAlloc< MappedUndo<T> > AllocType;
		MappedUndo(const AllocType& cc)
			: data_(cc)
		{

		}

		ObjectType& EmplaceBack()
		{
			return data_.emplace_back( ObjectType(data_.get_allocator()) );
		}

		std::deque< ObjectType, DBAlloc<ObjectType> > data_;
	};
}