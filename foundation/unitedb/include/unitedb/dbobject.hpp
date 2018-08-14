/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/dbtypes.hpp>
#include <boost/multi_index_container.hpp> 
#include <boost/multi_index/ordered_index.hpp> 
#include <boost/multi_index/member.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

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



#define BODY_DB_FIELD(_t, _v, _def) _t _v = _def;
#define CONSTR_DB_FIELD(_t, _v, _def) // empty.

#define BODY_DB_MFIELD(_t, _v) _t _v;
#define CONSTR_DB_MFIELD(_t, _v) , _v(al)


#define DB_MACRO_CAT(r, data, elem) BOOST_PP_CAT(data, elem)


// : _name##_Super() BOOST_PP_SEQ_FOR_EACH(DB_MACRO_CAT, CONSTR_, _args)

#define _DBOBJ_CONSTR_(_name, _args)\
template<typename T> _name(DBAlloc<T> al) {}\
template<typename C, typename T> _name(C&& c, DBAlloc<T> al)\
: _name##_Super() BOOST_PP_SEQ_FOR_EACH(DB_MACRO_CAT, CONSTR_, _args)\
{ c(*this); }

#define _DBOBJ_DEF_(_args) BOOST_PP_SEQ_FOR_EACH(DB_MACRO_CAT, BODY_, _args)


#define _DBOBJ_BODY_(_name, _args)  \
public:\
_DBOBJ_CONSTR_(_name, _args)\
_DBOBJ_DEF_(_args)



#define DBOBJ_CLASS(_objname, _typecode )  \
class _objname;\
using _objname##_Super = unitedb::DBObject<_objname, _typecode>;\
class _objname : public unitedb::DBObject<_objname, _typecode>

#define DBOBJ_BODY(_name, _args)  _DBOBJ_BODY_(_name, _args)

//DB_FIELD(_t, _v, _def) _t _v = _def;
//DB_MFIELD(_t, _v) _t _v;

