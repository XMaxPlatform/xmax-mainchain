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
		typedef std::deque< ObjectType, DBAlloc<ObjectType> > ArrayType;
		MappedUndo(const AllocType& cc)
			: data_(cc)
		{

		}
		const ObjectType& GetBack() const
		{
			return data_.back();
		}

		int64_t Size() const
		{
			return data_.size();
		}

		ObjectType& EmplaceBack()
		{
			return data_.emplace_back( ObjectType(data_.get_allocator()) );
		}

		void PopBack()
		{
			data_.pop_back();
		}

		ArrayType data_;
	};
}