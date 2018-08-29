/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <chaintypes.hpp>

namespace unitedb
{
	class Database;
}

namespace chain
{
	void InitSystemTables(unitedb::Database& db);
}