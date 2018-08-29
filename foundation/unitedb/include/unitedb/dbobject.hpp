/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <unitedb/dbtypes.hpp>
#include <unitedb/dbindexed.hpp>


namespace unitedb
{
	template<typename _Obj>
	class TObjectID
	{
	public:
		TObjectID(ObjIDCode i = 0) :val_(i) {}

		TObjectID& operator++() 
		{ 
			++val_; 
			return *this; 
		}

		friend bool operator < (const TObjectID& a, const TObjectID& b) { return a.val_ < b.val_; }
		friend bool operator > (const TObjectID& a, const TObjectID& b) { return a.val_ > b.val_; }
		friend bool operator == (const TObjectID& a, const TObjectID& b) { return a.val_ == b.val_; }
		friend bool operator != (const TObjectID& a, const TObjectID& b) { return a.val_ != b.val_; }
		friend std::ostream& operator<<(std::ostream& s, const TObjectID& id) {
			s << boost::core::demangle(typeid(TObjectID<T>).name()) << '(' << id.val_ << ')'; return s;
		}

		const ObjIDCode& GetValue() const
		{
			return val_;
		}
	private:
		ObjIDCode val_ = 0;
	};

	template<typename _Obj, ObjectTypeCode _Type>
	class TDBObject : public DBObjectBase
	{
	public:
		typedef TObjectID<_Obj> ObjectID;
		TDBObject() = default;
		template<typename T>
		TDBObject(DBAlloc<T>)
		{

		}

		ObjectID GetID() const
		{
			return ObjectID(getCode());
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
		typedef T ObjType;
		const ObjType * operator ->() const
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

		const ObjType& Get() const
		{
			DB_ASSERT_MSG(ptr_ != nullptr, "empty obj.");
			return *ptr_;
		}

		const ObjType* Ptr() const
		{
			return ptr_;
		}

		static ObjPtr<ObjType> MakePtr(const ObjType* p)
		{
			return ObjPtr<ObjType>(p);
		}
	private:

		ObjPtr() = default;

		ObjPtr(const ObjType* p)
			: ptr_(p)
		{
		}

		const ObjType * ptr_ = nullptr;
	};


}


// macros for DB_FIELD 
#define _DBBODY_FIELD_(_t, _v, _def) _t _v = _def;
#define _DBCONSTR_FIELD_(_t, _v, _def) // empty.

// macros for DB_SFIELD 
#define _DBBODY_SFIELD_(_t, _v) _t _v;
#define _DBCONSTR_SFIELD_(_t, _v) // empty.

// macros for DB_MFIELD 
#define _DBBODY_MFIELD_(_t, _v) _t _v;
#define _DBCONSTR_MFIELD_(_t, _v) , _v(al)

#define _DB_MACRO_CAT_(r, data, elem) BOOST_PP_CAT(data, elem)

#define _DBOBJ_CONSTR_(_name, _args)\
template<typename T> _name(unitedb::DBAlloc<T> al)\
: _name##_Super() BOOST_PP_SEQ_FOR_EACH(_DB_MACRO_CAT_, _DBCONSTR, _args) {}\
template<typename C, typename T> _name(C&& c, unitedb::DBAlloc<T> al)\
: _name##_Super() BOOST_PP_SEQ_FOR_EACH(_DB_MACRO_CAT_, _DBCONSTR, _args)\
{ c(*this); }


#define _DBOBJ_DEF_(_args) BOOST_PP_SEQ_FOR_EACH(_DB_MACRO_CAT_, _DBBODY, _args)


#define _DBOBJ_BODY_(_name, _args)  \
public:\
_DBOBJ_CONSTR_(_name, _args)\
_DBOBJ_DEF_(_args)\
private:



#define DBOBJ_CLASS(_objname, _typecode )  \
class _objname;\
using _objname##_Super = unitedb::TDBObject<_objname, _typecode>;\
class _objname : public unitedb::TDBObject<_objname, _typecode>

#define DBOBJ_BODY(_name, _args) _DBOBJ_BODY_(_name, _args)

// define field of simple value. e.g. int.
#define DB_FIELD(_type, _name, _default_value) _FIELD_(_type, _name, _default_value) 
// define field of simple value width out default value. e.g. int.
#define DB_SFIELD(_type, _name) _SFIELD_(_type, _name)
// define field of mapped class . e.g. MVector.
#define DB_MFIELD(_type, _name) _MFIELD_(_type, _name)

