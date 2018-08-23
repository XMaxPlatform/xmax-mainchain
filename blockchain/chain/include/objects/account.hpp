/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <objects/objectbase.hpp>

namespace chain
{
	DBOBJ_CLASS(Account, ObjectType::AccountObject)
	{
		DBOBJ_BODY(Account,
			(DB_SFIELD(AccountName, name))
		)
	};

	struct ByName;
	using AccountIdx = IndexedBy<Account,
		indexed::ordered_unique<indexed::tag<ByName>, indexed::member<Account, AccountName, &Account::name>>

	>;

	using AccountTable = db::DBTable<AccountIdx>;
}