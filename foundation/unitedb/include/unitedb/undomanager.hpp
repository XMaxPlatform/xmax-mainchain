/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/typebase.hpp>
#include <unitedb/undo.hpp>
#include <unitedb/dbundo.hpp>
#include <unitedb/dbundoop.hpp>
#include <deque>

namespace unitedb
{

	class UndoManager;

	class UndoOpStack
	{
	public:
		UndoOpStack(DBAlloc<char> alloc)
			: cache_(alloc)
			, last_revision_(0)
		{

		}
		MappedUndo<UndoOp> cache_;
		UndoRevision last_revision_;
	};

	class FUndo : public IGenericUndo
	{
	public:
		FUndo(UndoManager* owner, UndoRevision rev);

		virtual void Undo();
		virtual void Cancel();

		virtual UndoRevision GetRevision() const override
		{
			return revision_;
		}

	private:
		UndoRevision revision_ = 0;
		UndoManager* owner_ = nullptr;
	};

	class UndoManager
	{
	public:
		template<typename Alloc>
		UndoManager(IDatabase* owner, const Alloc& cc)
			: owner_(owner)
		{
		}

		void Init();

		IGenericUndo* StartUndo();

		void PushUndo(const UndoOpArg& arg);

		void LastUpdateFailure(ObjIDCode id);

		UndoRevision TopRevision() const
		{
			return stack_->last_revision_;
		}

	private:
		IDatabase* owner_ = nullptr;
		UndoOpStack* stack_ = nullptr;
	};

}