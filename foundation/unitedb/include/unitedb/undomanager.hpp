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
#include <vector>

namespace unitedb
{

	class UndoManager;

	class UndoOpStack
	{
	public:
		UndoOpStack(DBAlloc<char> alloc)
			: cache_(alloc)
		{

		}
		MappedUndo<UndoOp> cache_;
		UndoRevision last_commit_ = 0;
		UndoRevision last_undo_ = 0;
	};

	class FUndo : public IGenericUndo
	{
	public:
		typedef uint64_t UndoID;
		virtual void Undo() override;
		virtual void Cancel() override;
		virtual void Combine() override;

		virtual UndoRevision GetRevision() const override
		{
			return revision_;
		}

		inline UndoID GetID() const
		{
			return id_;
		}

		friend class UndoManager;
	private:
		FUndo(UndoManager* owner, UndoRevision rev);
		UndoRevision revision_ = 0;
		UndoManager* owner_ = nullptr;
		bool valid_ = true;
		UndoID id_ = 0;
		static UndoID scounter_;
	};

	struct UndoRecord
	{
		UndoRecord(FUndo* undo)
		{
			id_ = undo->GetID();
			rev_ = undo->GetRevision();
		}
		UndoRecord() = default;
		FUndo::UndoID id_ = 0;
		UndoRevision rev_ = InvalidRevision;
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
			return stack_->last_undo_;
		}

		void OnUndo(FUndo* undo);

		void OnCombine(FUndo* undo);

	private:
		bool popupRecord(FUndo::UndoID id, UndoRecord& out);
		void removeRecords(UndoRevision rev_begin);
		IDatabase* owner_ = nullptr;
		UndoOpStack* stack_ = nullptr;

		std::vector<UndoRecord> records_;
	};

}