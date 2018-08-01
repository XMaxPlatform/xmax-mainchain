/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbundoop.hpp>
namespace unitedb
{
	class ITable
	{
	public:
		virtual ~ITable() {}

		virtual IDBTable* GetDBTable() const = 0;

		virtual void StartUndo(DBRevision revision) = 0;

		virtual void Combine(DBRevision revision) = 0;

		virtual void Undo(const UndoOp& op) = 0;
	};

	class IDatabase
	{
	public:
		virtual ~IDatabase() {}

		virtual mapped_file::segment_manager* GetSegmentManager() const = 0;

		virtual mapped_file* GetMappdFile() const = 0;

		virtual DBRevision TopRevision() const = 0;

		virtual void PushUndo(const UndoOpArg& arg) = 0;

		virtual void LastUpdateFailure(ObjIDCode id) = 0;

		virtual void OnStartUndo(DBRevision revision) = 0;

		virtual void OnCombine(DBRevision revision) = 0;

		virtual ITable* GetTable(ObjectTypeCode code) = 0;

	};
}