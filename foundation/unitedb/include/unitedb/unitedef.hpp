/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/generictypes.hpp>
namespace unitedb
{
	class IDBTable
	{
	public:
		virtual ~IDBTable() {}
	};


	using ObjIDCode = int64_t;
	using ObjectTypeCode = uint16_t;
	using DBRevision = int64_t;

}