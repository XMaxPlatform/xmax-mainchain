/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <boost/interprocess/managed_mapped_file.hpp>
#include <memory>
#include <pro/io/file_system.hpp>

namespace unitedb
{
	namespace fs = pro::fs;
	namespace inpr = boost::interprocess;
	using mapped_file = inpr::managed_mapped_file;

	class Database
	{
	public:
		Database(const fs::path& dir, uint64_t managed_file_size);
		~Database();


	private:
		std::unique_ptr<mapped_file> db_file_;
		fs::path	db_path_;
		fs::path	db_file_path_;
	};
}