/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/dbundo.hpp>
#include <unitedb/dbundoop.hpp>
#include <stack>

namespace unitedb
{
	using UndoOpStack = std::stack< UndoOp, DBAlloc<UndoOp> >;


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
		UndoManager(const Alloc& cc)
			: stack_(cc)
		{

		}
		UndoSession StartUndo()
		{
			return UndoSession(nullptr);
		}

		void PushUndo(const UndoOpArg& arg)
		{

		}

		void PopUndo()
		{

		}

		UndoOpStack stack_;
	};

}