/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/ordered_index.hpp> 
#include <boost/multi_index/member.hpp>

namespace unitedb
{

	class DBObjBase
	{
	public:
		ObjIDCode __objid = 0;

		static inline ObjIDCode __getObjidcode(const DBObjBase& base)
		{
			return base.getCode();
		}

	protected:
		inline ObjIDCode getCode() const
		{
			return __objid;
		}
	};

	template<typename _Obj>
	class ObjectID
	{
	public:
		ObjectID(ObjIDCode i = 0) :val_(i) {}

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

		const ObjIDCode& GetValue() const
		{
			return val_;
		}
	private:
		ObjIDCode val_ = 0;
	};

	template<typename _Obj, ObjectTypeCode _Type>
	class DBObject : public DBObjBase
	{
	public:
		typedef ObjectID<_Obj> TypeID;
		DBObject() = default;
		template<typename T>
		DBObject(DBAlloc<T>)
		{

		}

		TypeID GetID() const
		{
			return TypeID(getCode());
		}

		ObjectTypeCode GetTypeCode() const
		{
			return TypeCode;
		}

		static const ObjectTypeCode TypeCode = _Type;
	};


	template<typename T>
	class ObjPtr
	{
	public:
		const T * operator ->() const
		{
			return ptr_;
		}
		explicit operator bool() const
		{
			return Valid();
		}

		bool Valid() const
		{
			return ptr_ != nullptr;
		}
		bool Empty() const
		{
			return ptr_ == nullptr;
		}

		const T& Get() const
		{
			DB_ASSERT_MSG(ptr_ != nullptr, "empty obj.");
			return *ptr_;
		}

		const T* Ptr() const
		{
			return ptr_;
		}

		static ObjPtr<T> MakePtr(const T* p)
		{
			return ObjPtr<T>(p);
		}
	private:

		ObjPtr() = default;

		ObjPtr(const T* p)
			: ptr_(p)
		{
		}

		const T * ptr_ = nullptr;
	};


}


#define DBOBJECT_CLASS(_objname, _typecode )\
class _objname : public unitedb::DBObject<_objname, _typecode>
