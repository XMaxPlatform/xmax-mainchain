/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#include <unitedb/dbundo.hpp>
namespace unitedb
{


	void UndoPatch::Undo()
	{
		if (Valid())
		{
			undo_->Undo();
		}

	}

	void UndoPatch::Cancel()
	{
		if (Valid())
		{
			undo_->Cancel();
		}
	}

	void UndoPatch::Combine()
	{
		if (Valid())
		{
			undo_->Combine();
		}
	}

	DBRevision UndoPatch::GetRevision() const
	{
		if (Valid())
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