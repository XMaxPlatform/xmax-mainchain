#pragma once

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

namespace chain
{
	namespace bip = boost::interprocess;

	template<typename T>
	using allocator = bip::allocator<T, bip::managed_mapped_file::segment_manager>;

	template<typename T>
	using mapped_vector = boost::interprocess::vector<T, allocator<T>>;
}
