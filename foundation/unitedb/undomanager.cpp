/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <unitedb/undomanager.hpp>

namespace unitedb
{
	FUndo::FUndo(UndoManager* owner, UndoRevision rev)
		: owner_(owner)
		, revision_(rev)
	{

	}
	void FUndo::Undo()
	{

	}
	void FUndo::Cancel()
	{

	}

	IGenericUndo* UndoManager::StartUndo()
	{
		owner_->EnableUndo(true);
		FUndo* undo = new FUndo(this, stack_->last_revision_);
		++stack_->last_revision_;
		return undo;
	}

	void UndoManager::Init()
	{
		std::string type_name = boost::core::demangle( typeid(UndoManager).name() ) + "UndoCache";

		stack_ = owner_->GetMappdFile()->find_or_construct< UndoOpStack >( type_name.c_str() ) (DefAlloc(owner_->GetSegmentManager()) );
	}

	void UndoManager::PushUndo(const UndoOpArg& arg)
	{
		stack_->cache_.EmplaceBack().Set(arg.op_, arg.objid_, arg.table_);
	}

	void UndoManager::LastUpdateFailure(ObjIDCode id)
	{
		BOOST_ASSERT(stack_->cache_.GetBack().id_ == id);
		BOOST_ASSERT(stack_->cache_.GetBack().op_ == UndoOp::Update);

		stack_->cache_.PopBack();
	}
}