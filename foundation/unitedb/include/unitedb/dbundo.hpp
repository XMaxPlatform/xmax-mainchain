/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <unitedb/dbundoop.hpp>
#include <stack>

namespace unitedb
{
	using UndoOpStack = std::stack< UndoOp, DBAlloc<UndoOp> >;

	class UndoManager
	{
	public:
		template<typename Alloc>
		UndoManager(const Alloc& cc)
			: stack_(cc)
		{

		}
		UndoOpStack stack_;
	};

}