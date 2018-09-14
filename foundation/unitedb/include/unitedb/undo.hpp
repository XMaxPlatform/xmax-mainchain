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
		typedef MDeque< ObjectType > ArrayType;

		typedef DefAlloc AllocType;
		MappedUndo() = delete;
		template<typename T>
		MappedUndo(DBAlloc<T> cc)
			: data_(ArrayType::allocator_type(cc.get_segment_manager()))
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

		void Remove(int64_t begidx, int64_t count)
		{
			ArrayType::const_iterator beg = data_.begin() + begidx;
			ArrayType::const_iterator last = beg + count;
			data_.erase(beg, last);
		}

		void Clear()
		{
			data_.clear();
		}

		ArrayType data_;
	};
}