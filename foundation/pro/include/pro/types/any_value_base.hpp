/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/generictypes.hpp>
#include <pro/exception/exceptions.h>
#include <vector>
#include <cassert>

namespace pro
{
	//template< >
	//static int32_t& CastTo(void* v, AnyType::Code ccheck)
	//{
	//	return AnyType::Cast<int32_t, AnyType::Type_I32>(v, ccheck);
	//}

#define ANY_TYPE_BIND_TMPLT(_type) \
	AnyValue(_type v) {\
		assign(v); \
	}\
	AnyValue& operator=(_type v) {\
		clearImpl();\
		assign(v); \
		return *this;\
	}


#define ANY_TYPE_CAST_TMPLT(_type, _code) \
		template<>\
		static _type& CastTo(void* v, AnyType::Code ccheck) {\
			return AnyType::Cast<_type, _code>(v, ccheck);\
		}\

#define SIMPLE_ASSIGN_TMPLT(_type, _code)\
	void AnyValue::assign(_type v) {\
		newSimpleType<decltype(v), _code>() = v;\
	}

	class DataStream
	{
	public:
		std::vector<char> data;
	};

	class IAnyContainer
	{
	public:
		typedef void* ValueBasePtr;
		typedef void ValueType;
		virtual ~IAnyContainer() {};

		friend class AnyValue;

		virtual ValueBasePtr GetValuePtr() const = 0;
		virtual void CopyFrom(const IAnyContainer* container) = 0;
		virtual IAnyContainer* CopySelf() const = 0;

		template<typename T>
		T& CastValue()
		{
			return *reinterpret_cast<T*>(GetValuePtr());
		}
		template<typename T>
		const T& CastValue() const
		{
			return *reinterpret_cast<const T*>(GetValuePtr());
		}
	protected:

		template<typename T>
		static IAnyContainer* Create()
		{
			return new T();
		}
		static void Delete(IAnyContainer* c)
		{
			delete c;
		}


	};


	template<typename Any>
	class FAnyContainer : public IAnyContainer
	{
	public:
		typedef Any ValueType;
		typedef ValueType* ValuePtr;

		ValueType val_;

	protected:
		virtual IAnyContainer::ValueBasePtr GetValuePtr() const override
		{
			return reinterpret_cast<IAnyContainer::ValueBasePtr>(const_cast<ValuePtr>(&val_));
		}
		virtual void CopyFrom(const IAnyContainer* container) override
		{
			*this = *static_cast<const FAnyContainer<ValueType>*>(container);
		}

		virtual IAnyContainer* CopySelf() const override
		{
			IAnyContainer* n = Create<FAnyContainer<Any>>();
			n->CopyFrom(this);
			return n;
		}
	};



	class AnyType
	{	
	public:
		typedef FAnyContainer<string> FString;
		typedef FAnyContainer<DataStream> FStream;
		typedef IAnyContainer*	Container;
		typedef double			Simple;
		enum Code
		{
			Type_Void = 0,
			Type_Bool,
			Type_I32,
			Type_UI32,
			Type_I64,
			Type_UI64,
			Type_F64,
			Type_String,
			Type_Stream,
			Type_Count,
		};
		union Data
		{
			bool		i8;
			int32_t		i32;
			uint32_t	ui32;
			uint64_t	u64;
			int64_t		i64;
			double		f64;
			Simple		anyval;
			Container	anyptr;
		};
		friend class AnyValue;
	private:
		template<AnyType::Code code>
		static constexpr bool IsSimpleType()
		{
			if constexpr (code == AnyType::Type_Stream || code == AnyType::Type_String)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		static bool IsSimpleType(AnyType::Code code)
		{
			if (code == AnyType::Type_Stream || code == AnyType::Type_String)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		template<typename T, typename D>
		static inline T& AsValue(D& d)
		{
			static_assert(sizeof(T) <= sizeof(AnyType::Data));
			return reinterpret_cast<T&>(d);
		}

		template<typename T, typename D>
		static inline T* AsPtr(D* d)
		{
			return reinterpret_cast<T*>(d);
		}

		template<typename T, AnyType::Code c>
		static T& Cast(void* v, AnyType::Code ccheck)
		{
			if (c != ccheck)
			{
				PRO_EXCEPT_WITH_DESC(FormatException, "nonsupport format.");
			}

			if constexpr (IsSimpleType<c>())
			{
				return *reinterpret_cast<T*>(v);
			}
			else
			{
				IAnyContainer** _any = reinterpret_cast<IAnyContainer**>(v);
				return (*_any)->CastValue<T>();
			}
		}

		template<typename T>
		static T& CastTo(void*, AnyType::Code)
		{
			static_assert(0, "nonsupport format.");
			PRO_EXCEPT_WITH_DESC(FormatException, "nonsupport format.");
			return *((T*)0);
		}
		//template< >
		//static int32_t& CastTo(void* v, AnyType::Code ccheck)
		//{
		//	return AnyType::Cast<int32_t, AnyType::Type_I32>(v, ccheck);
		//}

		ANY_TYPE_CAST_TMPLT(bool, AnyType::Type_Bool)
			ANY_TYPE_CAST_TMPLT(int32_t, AnyType::Type_I32)
			ANY_TYPE_CAST_TMPLT(uint32_t, AnyType::Type_UI32)
			ANY_TYPE_CAST_TMPLT(int64_t, AnyType::Type_I64)
			ANY_TYPE_CAST_TMPLT(uint64_t, AnyType::Type_UI64)
			ANY_TYPE_CAST_TMPLT(double, AnyType::Type_F64)
			ANY_TYPE_CAST_TMPLT(string, AnyType::Type_String)
			ANY_TYPE_CAST_TMPLT(DataStream, AnyType::Type_Stream)

	};

}