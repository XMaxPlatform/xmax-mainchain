/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <unitedb/dbtypes.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <memory>
#include <pro/io/file_system.hpp>
#include <unitedb/dbtable.hpp>

namespace unitedb
{
	class Database
	{
	public:
		template<typename T>
		class TableInst : public T
		{
		public:
			typedef typename T::MappedPtr MappedPtr;
			TableInst(MappedPtr p)
				: T(p)
			{
			}
		};

		enum InitFlag
		{
			NoFlag = 0,
			Discard = 1,
		};

		Database(const fs::path& dir, uint64_t managed_file_size);
		Database(const fs::path& dir, uint64_t managed_file_size, InitFlag flag);
		~Database();

		template<typename TableType>
		void InitTable()
		{
			ObjectTypeCode typeCode = TableType::ObjectType::TypeCode;

			std::string tableName = TableType::TableName();


			if (typeCode < tables_.size() && tables_[typeCode]) {
				BOOST_THROW_EXCEPTION(std::logic_error(tableName + "::TypeCode is already in use"));
			}
			TableType::MappedPtr ptr = db_file_->find_or_construct< TableType::MappedIndex >(tableName.c_str()) ( TableType::AllocType(GetSegmentManager()) );

			if (tables_.size() <= typeCode)
			{
				tables_.resize(typeCode + 1);
			}
			tables_[typeCode].reset(new TableInst<TableType>(ptr));
		}

		mapped_file::segment_manager* GetSegmentManager() const
		{
			return db_file_->get_segment_manager();
		}

		template<typename TableType>
		TableType* GetTable() const
		{
			return static_cast<TableType*>(tables_[TableType::ObjectType::TypeCode].get());
		}

		void Flush();
	private:

		void init(const fs::path& dir, uint64_t managed_file_size);

		std::unique_ptr<mapped_file> db_file_;
		fs::path	db_path_;
		fs::path	db_file_path_;

		std::vector< std::unique_ptr<ITable> > tables_;
	};
}