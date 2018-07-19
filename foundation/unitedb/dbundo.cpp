/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#include <unitedb/dbundo.hpp>
namespace unitedb
{


	void UndoSession::Undo()
	{
		if (undo_)
		{
			undo_->Undo();
		}

	}

	void UndoSession::Cancel()
	{
		if (undo_)
		{
			undo_->Cancel();
		}
	}
	UndoRevision UndoSession::GetRevision() const
	{
		if (undo_)
		{
			return undo_->GetRevision();
		}
		return InvalidRevision;
	}
	UndoSession::~UndoSession()
	{
		Undo();
	}

	UndoSession::UndoSession(UndoSession&& s)
	{
		undo_.swap(s.undo_);
	}

	UndoSession::UndoSession(IGenericUndo* ptr)
		: undo_(ptr)
	{

	}
}