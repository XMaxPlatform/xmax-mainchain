/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <boost/interprocess/managed_mapped_file.hpp>
#include <memory>

namespace unitedb
{
	namespace inpr = boost::interprocess;
	using managed_file = inpr::managed_mapped_file;

	class Database
	{
	public:
		Database();
		~Database();


	private:
		std::unique_ptr<managed_file>                        managed_file_;
	};
}