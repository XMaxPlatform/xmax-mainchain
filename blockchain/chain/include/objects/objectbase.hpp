/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <chaintypes.hpp>
#include <unitedb/dbtable.hpp>
namespace chain
{
	namespace db = unitedb;

	namespace indexed = db::multi_index;

	class ObjectType
	{
	public:
		enum Code
		{
			NullObject = 0,
			AccountObject,
		};
	};
}