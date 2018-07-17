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

	void UndoManager::Init()
	{
		std::string type_name = boost::core::demangle( typeid(UndoManager).name() ) + "UndoCache";

		stack_ = owner_->GetMappdFile()->find_or_construct< UndoOpStack >( type_name.c_str() ) ( UndoOpStack::AllocType(owner_->GetSegmentManager()) );
	}

	void UndoManager::PushUndo(const UndoOpArg& arg)
	{

	}

	void UndoManager::PopUndo()
	{

	}
}