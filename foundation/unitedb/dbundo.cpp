/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#include <unitedb/dbundo.hpp>
namespace unitedb
{


	void DBUndoPatch::Undo()
	{
		if (undo_)
		{
			undo_->Undo();
		}

	}

	void DBUndoPatch::Cancel()
	{
		if (undo_)
		{
			undo_->Cancel();
		}
	}

	void DBUndoPatch::Combine()
	{
		if (undo_)
		{
			undo_->Combine();
		}
	}

	DBRevision DBUndoPatch::GetRevision() const
	{
		if (undo_)
		{
			return undo_->GetRevision();
		}
		return InvalidRevision;
	}
	DBUndoPatch::~DBUndoPatch()
	{
		Undo();
	}

	DBUndoPatch::DBUndoPatch(DBUndoPatch&& s)
	{
		undo_.swap(s.undo_);
	}

	DBUndoPatch::DBUndoPatch(IGenericUndo* ptr)
		: undo_(ptr)
	{

	}
}