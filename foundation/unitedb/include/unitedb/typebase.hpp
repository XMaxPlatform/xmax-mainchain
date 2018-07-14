/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

namespace unitedb
{
	class IDBTable
	{
	public:
		virtual ~IDBTable() {}
	};

	class IDatabase
	{
	public:
		virtual ~IDatabase() {}

		virtual mapped_file::segment_manager* GetSegmentManager() const = 0;
	};

}