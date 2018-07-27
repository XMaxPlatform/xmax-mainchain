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

		
		// Constructs by base types.
		ANY_TYPE_BIND_TMPLT(bool)
			ANY_TYPE_BIND_TMPLT(int32_t)
			ANY_TYPE_BIND_TMPLT(uint32_t)
			ANY_TYPE_BIND_TMPLT(int64_t)
			ANY_TYPE_BIND_TMPLT(uint64_t)
			ANY_TYPE_BIND_TMPLT(double)
			ANY_TYPE_BIND_TMPLT(const string&)
			ANY_TYPE_BIND_TMPLT(const DataStream&)
			ANY_TYPE_BIND_TMPLT(const AnyValue&)
			ANY_TYPE_BIND_TMPLT(const char*)

	public:
		/**
		* Constructs by string.
		* @param[in] v string.
		*/
		AnyValue(string&& v);
		/**
		* Constructs by DataStream.
		* @param[in] v DataStream.
		*/
		AnyValue(DataStream&& v);
		/**
		* Constructs by binary data.
		* @param[in] data data ptr.
		* @param[in] len data size.
		*/
		AnyValue(const void* data, size_t len);

		/**
		* Constructs right AnyValue.
		* @param[in] v right AnyValue.
		*/
		AnyValue(AnyValue&& v);
		/**
		* Constructs with null.
		*/
		AnyValue();

		/**
		* operator = AnyValue&&, and return self.
		* @param[in] v right AnyValue.
		* @return self.
		*/
		AnyValue & operator=(AnyValue&&);

		~AnyValue();

		/**
		* set by a value.
		* @param[in] v any value.
		*/
		template<typename T>
		void SetValue(const T& v)
		{
			Clear();
			assign(v);
		}
		/**
		* set by a right value.
		* @param[in] v any value.
		*/
		template<typename T>
		void SetValue(T&& v)
		{
			Clear();
			assign(std::forward<T>(v));
		}
		/**
		* set by binary data.
		* @param[in] data data ptr.
		* @param[in] len data size.
		*/
		void SetValue(const void* data, size_t len)
		{
			Clear();
			assign(data, len);
		}

		/**
		* clear.
		*/
		void Clear();

		/**
		* convert to string.
		* @return string.
		*/
		string ToString() const;

		/**
		* convert to string.
		* @param[out] str out string.
		*/
		void ToString(string& str) const;
		/**
		* get value type.
		* @return the type of value.
		*/
		inline AnyType::Code GetType() const
		{
			return code_;
		}
		/**
		* return if empty or not.
		* @return the result.
		*/
		inline bool IsEmpty() const
		{
			return (AnyType::Type_Void == code_);
		}
		/**
		* return if valid or not.
		* @return the result.
		*/
		inline bool IsValid() const
		{
			return (AnyType::Type_Void != code_);
		}
		/**
		* check the value type.
		* @param[in] code value type.
		* @return the result.
		*/
		inline bool IsType(AnyType::Code code) const
		{
			return (code == code_);
		}
		/**
		* check the value type.
		* @param[in] c value type.
		* @return the result.
		*/
		template<AnyType::Code c>
		inline bool IsType() const
		{
			return IsType(c);
		}
		/**
		* cast to a type.
		* cast to a type, throw when cast false.
		* @param[in] T value type.
		* @return the result.
		*/
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
		void forceNone();
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