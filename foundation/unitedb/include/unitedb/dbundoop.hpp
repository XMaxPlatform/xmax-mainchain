/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>

namespace unitedb
{
	class UndoOp
	{
	public:
		enum UndoCode
		{
			None = 0,
			Create = 1,
			Update = 2,
			Delete = 3,
		};

		UndoCode op_ = UndoOp::None;
		int32_t tag_ = 0;
		

	};

	class UndoOpArg
	{
	public:
		UndoOp::UndoCode op_ = UndoOp::None;
		ObjIDCode objid_ = 0;
		int32_t tag_ = 0;

		UndoOpArg(UndoOp::UndoCode op, ObjIDCode id)
			: op_(op)
			, objid_(id)
		{

		}

	};
}