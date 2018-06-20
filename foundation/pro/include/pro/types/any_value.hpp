/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>

#include <memory>
#include <vector>
#include <pro/types/generictypes.hpp>

namespace pro
{

	class AnyVaule
	{
	public:
		enum TypeCode
		{
			UI64,
			I64,
			Void,
		};
		union Entity
		{
			uint64_t	u64;
			int64_t		i64;
			void*		point;
		};
		AnyVaule() = default;
		AnyVaule(const AnyVaule&) = delete;
		AnyVaule& operator = (const AnyVaule &) = delete;


	};

}