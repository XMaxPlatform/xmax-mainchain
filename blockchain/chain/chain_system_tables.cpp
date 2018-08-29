/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#include <unitedb/database.hpp>
#include <chain_tables.hpp>
#include <tables/account.hpp>

namespace chain
{
	void InitSystemTables(unitedb::Database& db)
	{
		db.InitTable<AccountTable>();
	}
}