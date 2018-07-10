/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <unitedb/unitedef.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <memory>
#include <pro/io/file_system.hpp>
#include <unitedb/dbbase.hpp>

namespace unitedb
{
	class Database
	{
	public:
		struct AnyTable 
		{
			AnyTable()
				: ptr_(nullptr)
			{

			}

			bool IsEmpty() const
			{
				return ptr_ == nullptr;
			}
			bool IsValid() const
			{
				return ptr_ != nullptr;
			}

			void Set(void* p)
			{
				ptr_ = p;
			}
		private:
			void* ptr_;
		}; 

		Database(const fs::path& dir, uint64_t managed_file_size);
		~Database();

		template<typename TableType>
		void InitTable()
		{
			ObjectTypeCode typeCode = TableType::ObjectType::TypeCode;

			std::string tableName = TableType::TableName();
			if (typeCode < tables_.size()  && tables_[typeCode].IsValid()) {
				BOOST_THROW_EXCEPTION(std::logic_error(tableName + "::TypeCode is already in use"));
			}
			TableType* ptr = db_file_->find_or_construct< TableType >(tableName.c_str()) ( TableType::AllocType(db_file_->get_segment_manager()) );

			if (tables_.size() <= typeCode)
			{
				tables_.resize(typeCode + 1);
			}
			tables_[typeCode].Set(ptr);
		}


	private:
		std::unique_ptr<mapped_file> db_file_;
		fs::path	db_path_;
		fs::path	db_file_path_;

		std::vector<AnyTable> tables_;
	};
}