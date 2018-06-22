/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/generictypes.hpp>

namespace pro
{
	/**
	* This class encapsulates some basic types 
	*/
	class AnyVaule
	{
	public:
		enum TypeCode
		{
			Type_Void = 0,
			Type_I32,
			Type_UI32,
			Type_UI64,
			Type_I64,
			Type_F64,
			Type_String
			
		};
		~AnyVaule();
		AnyVaule();
		AnyVaule(int32_t v);
		AnyVaule(uint32_t v);
		AnyVaule(int64_t v);
		AnyVaule(uint64_t v);
		AnyVaule(double v);
		AnyVaule(const string& v);
		AnyVaule(const char* v);


		template<typename T>
		void SetValue(const T& v)
		{
			Clear();
			assign(v);
		}

		void Clear();

	protected:

		void assign(int32_t v);

		void assign(uint32_t v);

		void assign(int64_t v);

		void assign(uint64_t v);

		void assign(double v);

		void assign(const string& v);

		void assign(const char* v);

		void clearImpl();

		template<typename T>
		inline void setValue(T v, TypeCode c)
		{
			static_assert(sizeof(T) <= 8);
			*reinterpret_cast<T*>(this) = v;
			code_ = c;
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
		};
		//std::variant<int32_t, uint32_t, int64_t, uint64_t, string, double> val;
		Data val_;
		uint16_t subcode_;
		TypeCode code_;
	};

}