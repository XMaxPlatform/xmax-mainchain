/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/any_value_base.hpp>


namespace pro
{

	/**
	* This class encapsulates some basic types 
	*/
	class AnyValue
	{
	public:

		~AnyValue();
		AnyValue();
		AnyValue(bool v);
		AnyValue(int32_t v);
		AnyValue(uint32_t v);
		AnyValue(int64_t v);
		AnyValue(uint64_t v);
		AnyValue(double v);
		AnyValue(string&& v);
		AnyValue(const string& v);
		AnyValue(DataStream&& v);
		AnyValue(const DataStream& v);
		AnyValue(const void* data, size_t len);
		AnyValue(const char* v);
		AnyValue(const AnyValue& v);
		AnyValue(AnyValue&& v);

		AnyValue & operator=(const AnyValue&);
		AnyValue & operator=(AnyValue&&);

		template<typename T>
		void SetValue(const T& v)
		{
			Clear();
			assign(v);
		}
		template<typename T>
		void SetValue(T&& v)
		{
			Clear();
			assign(v);
		}

		void SetValue(const void* data, size_t len)
		{
			Clear();
			assign(data, len);
		}

		void Clear();

		inline AnyType::Code GetType() const
		{
			return code_;
		}

		inline bool IsEmpty() const
		{
			return (AnyType::Type_Void == code_);
		}
		inline bool IsValid() const
		{
			return (AnyType::Type_Void != code_);
		}

		inline bool IsType(AnyType::Code code) const
		{
			return (code == code_);
		}

		template<typename T>
		inline const T& CastTo() const
		{
			return AnyType::CastTo<T>((void*)(&val_.anyval), code_);
		}

	protected:
		SIMPLE_ASSIGN_TMPLT(bool, AnyType::Type_Bool);
		SIMPLE_ASSIGN_TMPLT(int32_t, AnyType::Type_I32);
		SIMPLE_ASSIGN_TMPLT(uint32_t, AnyType::Type_UI32);
		SIMPLE_ASSIGN_TMPLT(int64_t, AnyType::Type_I64);
		SIMPLE_ASSIGN_TMPLT(uint64_t, AnyType::Type_UI64);
		SIMPLE_ASSIGN_TMPLT(double, AnyType::Type_F64);

		void assign(const char* v);

		void assign(const string& v);
		void assign(string&& v);
		void assign(const DataStream& v);
		void assign(DataStream&& v);
		void assign(const void* data, size_t len);

		void assign(AnyValue&& v);
		void assign(const AnyValue& v);

		void clearImpl();
	
		template<typename T, AnyType::Code c>
		inline T& newSimpleType()
		{
			static_assert(AnyType::IsSimpleType<c>());
			code_ = c;

			return AnyType::AsValue<T>(val_.anyval);
		}

		template<typename T, AnyType::Code c>
		inline typename T::ValueType& newContainer()
		{
			static_assert(!AnyType::IsSimpleType<c>());
			//assert(val_.anyptr == nullptr);

			setContainerImpl(c, IAnyContainer::Create<T>());

			return *reinterpret_cast<T::ValuePtr>(val_.anyptr->GetValuePtr());
		}

		void setContainerImpl(AnyType::Code c, IAnyContainer* any)
		{
			code_ = c;
			val_.anyptr = any;
		}

	private:

		//std::variant<int32_t, uint32_t, int64_t, uint64_t, string, double> val;
		AnyType::Data val_;
		uint16_t subcode_;
		AnyType::Code code_;
	};

}