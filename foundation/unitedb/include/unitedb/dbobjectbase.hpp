/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>

namespace unitedb
{
	class DBObjectBase
	{
	public:
		ObjIDCode __objid = 0;

		static inline ObjIDCode __getObjidcode(const DBObjectBase& base)
		{
			return base.getCode();
		}

	protected:
		inline ObjIDCode getCode() const
		{
			return __objid;
		}
	};

}