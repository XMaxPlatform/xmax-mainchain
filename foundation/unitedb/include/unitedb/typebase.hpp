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

		virtual void SetUndo(bool set) = 0;
	};

	class IDatabase
	{
	public:
		virtual ~IDatabase() {}

		virtual mapped_file::segment_manager* GetSegmentManager() const = 0;

		virtual mapped_file* GetMappdFile() const = 0;

		virtual UndoRevision TopRevision() const = 0;

		virtual void PushUndo(const UndoOpArg& arg) = 0;

		virtual void LastUpdateFailure() = 0;

		virtual void EnableUndo(bool set) = 0;


	};
}