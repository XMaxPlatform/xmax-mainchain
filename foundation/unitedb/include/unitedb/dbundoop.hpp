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

		UndoOp(DBAlloc<char>)
		{

		}

		enum UndoCode
		{
			None = 0,
			Create = 1,
			Update = 2,
			Delete = 3,
		};

		UndoCode op_ = UndoOp::None;
		ObjIDCode id_ = 0;
		ObjectTypeCode table_ = 0;
		
		void Set(UndoCode op, ObjIDCode id, ObjectTypeCode table)
		{
			op_ = op;
			id_ = id;
			table_ = table;
		}

	};

	class UndoOpArg
	{
	public:
		UndoOp::UndoCode op_ = UndoOp::None;
		ObjIDCode objid_ = 0;
		ObjectTypeCode table_ = 0;

		UndoOpArg(UndoOp::UndoCode op, ObjIDCode id, ObjectTypeCode table)
			: op_(op)
			, objid_(id)
			, table_(table)
		{

		}

	};
}