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
		stack_->EmplaceBack().Set(arg.op_, arg.objid_, arg.table_);
	}

	void UndoManager::LastUpdateFailure(ObjIDCode id)
	{
		BOOST_ASSERT(stack_->GetBack().id_ == id);
		BOOST_ASSERT(stack_->GetBack().op_ == UndoOp::Update);

		stack_->PopBack();
	}
}