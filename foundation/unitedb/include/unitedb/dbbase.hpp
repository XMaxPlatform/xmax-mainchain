/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/unitedef.hpp>
#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/ordered_index.hpp> 
#include <boost/multi_index/member.hpp>

namespace unitedb
{

	typedef uint16_t ObjectTypeCode;
	namespace fs = pro::fs;
	namespace inpr = boost::interprocess;
	using mapped_file = inpr::managed_mapped_file;

	template<typename T>
	using DBAlloc = inpr::allocator<T, mapped_file::segment_manager>;

	template<typename _Obj>
	class ObjectID
	{
	public:
		ObjectID(int64_t i = 0) :val_(i) {}

		ObjectID& operator++() 
		{ 
			++val_; 
			return *this; 
		}

		friend bool operator < (const ObjectID& a, const ObjectID& b) { return a.val_ < b.val_; }
		friend bool operator > (const ObjectID& a, const ObjectID& b) { return a.val_ > b.val_; }
		friend bool operator == (const ObjectID& a, const ObjectID& b) { return a.val_ == b.val_; }
		friend bool operator != (const ObjectID& a, const ObjectID& b) { return a.val_ != b.val_; }
		friend std::ostream& operator<<(std::ostream& s, const ObjectID& id) {
			s << boost::core::demangle(typeid(ObjectID<T>).name()) << '(' << id.val_ << ')'; return s;
		}

		int64_t val_ = 0;
	};

	template<typename _Obj, ObjectTypeCode _Type>
	class DBObject
	{
	public:

		ObjectID<_Obj> id_;
		static const ObjectTypeCode TypeCode = _Type;
	};



	class ITable
	{
	public:
		virtual ~ITable() {}
	};

	template<typename _multi_index>
	class DBTable : public ITable
	{
	public:
		typedef _multi_index MultiIndexType;
		typedef typename MultiIndexType::value_type ObjectType;
		typedef DBAlloc<DBTable> AllocType;

		static std::string TableName()
		{
			static std::string type_name = boost::core::demangle(typeid(typename ObjectType).name()) + "Table";
			return type_name;
		}

		DBTable(DBAlloc<ObjectType> alloc)
			//: indices_(alloc)
		{

		}

	private:
		//MultiIndexType indices_;

	};


}