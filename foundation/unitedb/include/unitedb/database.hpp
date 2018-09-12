/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <unitedb/dbtypes.hpp>
#include <unitedb/typebase.hpp>
#include <unitedb/table.hpp>
#include <unitedb/dbundo.hpp>

namespace unitedb
{
	class Database
	{
	public:

		enum InitFlag
		{
			NoFlag = 0,
			Discard = 1,
		};

		static Database* InitDB(const fs::path& dir, uint64_t managed_file_size);
		static Database* InitDB(const fs::path& dir, uint64_t managed_file_size, InitFlag flag);

		static void DestroyDB(Database* db);

		virtual ~Database() {};

		template<typename TableType>
		void InitTable()
		{
			ObjectTypeCode typeCode = TableType::ObjectType::TypeCode;

			std::string tableName = TableType::TableName();


			if (tableUsed(typeCode)) {
				DB_THROW(std::logic_error(tableName + "::TypeCode is already in use"));
			}
			TableType::MappedPtr ptr = getMappdFile()->find_or_construct< TableType::MappedIndex >(tableName.c_str()) ( TableType::AllocType(getMappdFile()->get_segment_manager()) );

			ptr->Check();

			ITable* table = new FTable<TableType>(castDatabase(), ptr);

			setTableInternal(typeCode, table);
		}

		template<typename TableType>
		inline TableType* GetTable() const
		{
			return static_cast<TableType*>(getTableInternal(TableType::ObjectType::TypeCode)->GetDBTable());
		}

		virtual DBPatch StartUndo() = 0;

		virtual void Flush() = 0;

		virtual void Close() = 0;

		virtual bool Commit(DBRevision rev) = 0;

		virtual DBRevision GetTopRevision() const = 0;

		virtual DBRevision GetLastCommit() const = 0;

		mapped_file::segment_manager* GetSegment() const
		{
			return getMappdFile()->get_segment_manager();
		}
	protected:
		virtual IDatabase* castDatabase() = 0;
		virtual mapped_file* getMappdFile() const = 0;
		virtual bool tableUsed(ObjectTypeCode code) const = 0;
		virtual ITable* getTableInternal(ObjectTypeCode code) const = 0;
		virtual void setTableInternal(ObjectTypeCode code, ITable*) = 0;
	};
}