/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>

#include <memory>
#include <vector>

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
			
		};
		union Vaule
		{
			int32_t		i32;
			uint32_t	ui32;
			uint64_t	u64;
			int64_t		i64;
			double		f64;
			char*		pointer;
		};
		AnyVaule();
		AnyVaule(const AnyVaule&) = delete;
		AnyVaule& operator = (const AnyVaule &) = delete;

		/**
		* Set value type as int32
		* @param[in] int32_t
		*/
		void Set(int32_t v);
		/**
		* Set value type as uint32
		* @param[in] uint32_t
		*/
		void Set(uint32_t v);
		/**
		* Set value type as int64
		* @param[in] int64_t
		*/
		void Set(int64_t v);
		/**
		* Set value type as uint64
		* @param[in] uint64_t
		*/
		void Set(uint64_t v);
		/**
		* Set value type as double
		* @param[in] double
		*/
		void Set(double v);


	protected:
		template<typename T>
		inline void setValue(T v)
		{
			static_assert(sizeof(T) <= 8);
			*reinterpret_cast<T*>(this) = v;
		}

		inline void setCode(TypeCode c)
		{
			code = c;
		}

	private:
		Vaule val;
		uint16_t subcode;
		TypeCode code;
	};

}