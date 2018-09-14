/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/unitedef.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <filesystem>

namespace unitedb
{
	namespace fs = std::filesystem;
	namespace inpr = boost::interprocess;
	using mapped_file = inpr::managed_mapped_file;
	using segment_manager = inpr::managed_mapped_file::segment_manager;



	template<typename T>
	using DBAlloc = inpr::allocator<T, segment_manager>;


	using DefAlloc = DBAlloc<char>;

	using MString = boost::interprocess::basic_string< char, std::char_traits< char >, DBAlloc< char > >; // mapped string.

	template<typename T>
	using MVector = boost::interprocess::vector<T, DBAlloc<T> >; // mapped vector.

	template<typename T>
	using MDeque = boost::interprocess::deque<T, DBAlloc<T> >; // mapped deque.

	constexpr DBRevision InvalidRevision = -1;

	class IDBTable
	{
	public:
		virtual ~IDBTable() {}

	protected:

	};



}

#define DB_ASSERT(expr) BOOST_ASSERT(expr)

#define DB_ASSERT_MSG(expr, msg) BOOST_ASSERT_MSG(expr, msg)

#define DB_THROW(expr) BOOST_THROW_EXCEPTION(expr)