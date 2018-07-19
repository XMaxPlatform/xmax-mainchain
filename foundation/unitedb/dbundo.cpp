/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#include <unitedb/dbundo.hpp>
namespace unitedb
{


	void UndoPatch::Undo()
	{
		if (undo_)
		{
			undo_->Undo();
		}

	}

	void UndoPatch::Cancel()
	{
		if (undo_)
		{
			undo_->Cancel();
		}
	}
	UndoRevision UndoPatch::GetRevision() const
	{
		if (undo_)
		{
			return undo_->GetRevision();
		}
		return InvalidRevision;
	}
	UndoPatch::~UndoPatch()
	{
		Undo();
	}

	UndoPatch::UndoPatch(UndoPatch&& s)
	{
		undo_.swap(s.undo_);
	}

	UndoPatch::UndoPatch(IGenericUndo* ptr)
		: undo_(ptr)
	{

	}
}