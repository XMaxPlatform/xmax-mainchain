/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <unitedb/undomanager.hpp>

namespace unitedb
{
	IGenericUndo* UndoManager::StartUndo()
	{
		owner_->EnableUndo(true);
		return nullptr;
	}

	void UndoManager::PushUndo(const UndoOpArg& arg)
	{
	}

	void UndoManager::PopUndo()
	{

	}
}