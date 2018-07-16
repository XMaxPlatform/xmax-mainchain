/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/typebase.hpp>
#include <unitedb/dbundo.hpp>
#include <unitedb/dbundoop.hpp>
#include <deque>

namespace unitedb
{
	using UndoOpStack = std::deque< UndoOp, DBAlloc<UndoOp> >;

	class FUndo : public IGenericUndo
	{
	public:
		virtual void Undo() override
		{

		}
		virtual void Cancel() override
		{

		}

		virtual UndoRevision GetRevision() const override
		{
			return revision_;
		}

	private:
		UndoRevision revision_ = 0;
	};

	class UndoManager
	{
	public:
		template<typename Alloc>
		UndoManager(IDatabase* owner, const Alloc& cc)
			: owner_(owner)
		{

		}
		IGenericUndo* StartUndo();

		void PushUndo(const UndoOpArg& arg);

		void PopUndo();

		IDatabase* owner_;
		UndoOpStack* stack_;

	};

}