/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#include <unitedb/dbundo.hpp>
namespace unitedb
{


	void DBPatch::Undo()
	{
		if (undo_)
		{
			undo_->Undo();
		}

	}

	void DBPatch::Push()
	{
		if (undo_)
		{
			undo_->Cancel();
		}
	}

	void DBPatch::Combine()
	{
		if (undo_)
		{
			undo_->Combine();
		}
	}

	DBRevision DBPatch::GetRevision() const
	{
		if (undo_)
		{
			return undo_->GetRevision();
		}
		return InvalidRevision;
	}
	DBPatch::~DBPatch()
	{
		Undo();
	}

	DBPatch::DBPatch(DBPatch&& s)
	{
		undo_.swap(s.undo_);
	}

	DBPatch::DBPatch(IGenericUndo* ptr)
		: undo_(ptr)
	{

	}
}