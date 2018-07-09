/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/unitedef.hpp>
namespace unitedb
{

	template<uint16_t TypeID>
	class DBObject
	{
	public:


		static const uint16_t type_id_ = TypeID;
	};



	class ITable
	{
	public:
		virtual ~ITable() {}
	};

	template<typename IndexType>
	class DBTable : public ITable
	{


	public:

	};


}