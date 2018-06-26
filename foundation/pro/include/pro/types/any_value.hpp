/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/generictypes.hpp>
#include <vector>

namespace pro
{


	class DataStream
	{
	public:
		std::vector<char> data;
	};


	/**
	* This class encapsulates some basic types 
	*/
	class AnyValue
	{
	public:
		enum TypeCode
		{
			Type_Void = 0,
			Type_Bool,
			Type_I32,
			Type_UI32,
			Type_UI64,
			Type_I64,
			Type_F64,
			Type_String,
			Type_Stream,
			
		};
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
		void SetValue(const void* data, size_t len)
		{
			Clear();
			assign(data, len);
		}

		void Clear();

		inline TypeCode GetType() const
		{
			return code_;
		}

		inline bool IsEmpty() const
		{
			return Type_Void == code_;
		}
		inline bool IsValid() const
		{
			return Type_Void != code_;
		}

		inline bool IsType(TypeCode code) const
		{
			return code == code_;
		}
	protected:
		void assign(bool v);
		void assign(int32_t v);
		void assign(uint32_t v);
		void assign(int64_t v);
		void assign(uint64_t v);
		void assign(double v);
		void assign(const char* v);

		void assign(const string& v);
		void assign(string&& v);
		void assign(const DataStream& v);
		void assign(DataStream&& v);
		void assign(const void* data, size_t len);

		void assign(AnyValue&& v);
		void assign(const AnyValue& v);

		void clearImpl();

		template<typename T>
		inline void setValue(T v, TypeCode c)
		{
			static_assert(sizeof(T) <= sizeof(Data));
			reinterpret_cast<T&>(val_) = v;
			code_ = c;
		}

		template<typename T>
		inline T& asValue() const
		{
			static_assert(sizeof(T) <= sizeof(Data));
			return reinterpret_cast<T&>(val_);
		}
		template<typename T>
		inline T* asPtr() const
		{
			return reinterpret_cast<T*>(val_.anyptr);
		}

		template<typename T>
		inline T& newType(TypeCode c)
		{
			val_.anyptr = new T;
			code_ = c;
			return *asPtr<T>();
		}

		inline void setCode(TypeCode c)
		{
			code_ = c;
		}

	private:
		union Data
		{
			int32_t		i32;
			uint32_t	ui32;
			uint64_t	u64;
			int64_t		i64;
			double		f64;
			string*		str;
			DataStream* stream;
			void*		anyptr;
		};
		//std::variant<int32_t, uint32_t, int64_t, uint64_t, string, double> val;
		Data val_;
		uint16_t subcode_;
		TypeCode code_;
	};

}