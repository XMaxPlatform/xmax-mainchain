/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/unitedef.hpp>
#include <pro/io/file_system.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
namespace unitedb
{
	typedef uint16_t ObjectTypeCode;
	namespace fs = pro::fs;
	namespace inpr = boost::interprocess;
	using mapped_file = inpr::managed_mapped_file;
	using segment_manager = inpr::managed_mapped_file::segment_manager;

	template<typename T>
	using DBAlloc = inpr::allocator<T, segment_manager>;


	using MappedString = std::basic_string< char, std::char_traits< char >, DBAlloc< char > >;

	template<typename T>
	using MappedVector = std::vector<T, DBAlloc<T> >;

}