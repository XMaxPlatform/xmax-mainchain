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

		AnyVaule();
		~AnyVaule();
		/**
		* Set value type as int32
		* @param[in] int32_t
		*/
		void SetValue(int32_t v);
		/**
		* Set value type as uint32
		* @param[in] uint32_t
		*/
		void SetValue(uint32_t v);
		/**
		* Set value type as int64
		* @param[in] int64_t
		*/
		void SetValue(int64_t v);
		/**
		* Set value type as uint64
		* @param[in] uint64_t
		*/
		void SetValue(uint64_t v);
		/**
		* Set value type as double
		* @param[in] double
		*/
		void SetValue(double v);

		void SetValue(const string& v);

		void SetValue(const char* v);

		void Clear();

	protected:

		template<typename T>
		inline void setValue(T v)
		{
			static_assert(sizeof(T) <= 8);
			*reinterpret_cast<T*>(this) = v;
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