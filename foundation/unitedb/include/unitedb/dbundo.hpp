/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <memory>
namespace unitedb
{
	class IGenericUndo
	{
	public:

		virtual void Undo() = 0;
		virtual void Cancel() = 0;

		virtual UndoRevision GetRevision() const = 0;
	};

	class UndoSession
	{
	public:

		void Undo()
		{
			undo_->Undo();
		}

		void Cancel()
		{
			undo_->Cancel();
		}
		UndoRevision GetRevision() const
		{
			return undo_->GetRevision();
		}
		~UndoSession()
		{
			Undo();
		}
		
		friend class FDatabase;
	protected:

		UndoSession(IGenericUndo* ptr)
			: undo_(ptr)
		{

		}

		std::shared_ptr<IGenericUndo> undo_;
	};

}