/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/generictypes.hpp>
namespace pro
{
	typedef char sglyph;
	typedef uint32_t ssymbol;
	typedef uint32_t sregion;

	struct SCodeRS
	{
		sregion region;
		ssymbol symbol;

		constexpr SCodeRS(sregion r, ssymbol s)
			: region(r)
			, symbol(s)
		{

		}
	};


	static constexpr sglyph SCODE_REGION_0[] = "#abcdefghijklmnopqrstuvwxyz.1234";
	static constexpr sglyph SCODE_REGION_1[] = "#@567890";

	static constexpr ssymbol SCODE_SYMBOL_EXPEND = 0;
	static constexpr ssymbol SR0_SYMBOL_A = 1;
	static constexpr ssymbol SR0_SYMBOL_Z = 26;
	static constexpr ssymbol SR0_SYMBOL_POINT = 27;
	static constexpr ssymbol SR0_SYMBOL_1 = 28;
	static constexpr ssymbol SR0_SYMBOL_4 = 31;
	static constexpr ssymbol SR1_SYMBOL_AT = 1;
	static constexpr ssymbol SR1_SYMBOL_5 = 2;
	static constexpr ssymbol SR1_SYMBOL_9 = 6;
	static constexpr ssymbol SR1_SYMBOL_0 = 7;

	static constexpr sregion SR0_NUM = 0;
	static constexpr sregion SR1_NUM = 1;
	static constexpr sregion SRERROR_NUM = -1;

	static constexpr uint16_t SCODE_REGION_0_SIZE = sizeof(SCODE_REGION_0) - 1;
	static constexpr uint16_t SCODE_REGION_1_SIZE = sizeof(SCODE_REGION_1) - 1;

	static constexpr sglyph GetSGlyph(sregion region, ssymbol sym)
	{		
		switch (region)
		{
		case 0:
		{
			if (0 < sym && sym < SCODE_REGION_0_SIZE)
			{
				return SCODE_REGION_0[sym];
			}
			break;
		}
		case 1:
		{			
			if (0 < sym && sym < SCODE_REGION_1_SIZE)
			{
				return SCODE_REGION_1[sym];
			}
			break;
		}

		default:
			break;
		}

		return (sglyph)0;
	}


	static constexpr SCodeRS GetSCodeRS(sglyph ch)
	{
		if ('a' <= ch && ch <= 'z')
		{
			return SCodeRS(SR0_NUM, (ssymbol)(ch - 'a' + SR0_SYMBOL_A));
		}
		if ('.' == ch)
		{
			return SCodeRS(SR0_NUM, SR0_SYMBOL_POINT);
		}
		if ('1' <= ch && ch <= '4')
		{
			return SCodeRS(SR0_NUM, (ssymbol)(ch - '1' + SR0_SYMBOL_1));
		}
		if ('@' == ch)
		{
			return SCodeRS(SR1_NUM, SR1_SYMBOL_AT);
		}
		if ('5' <= ch && ch <= '9')
		{
			return SCodeRS(SR1_NUM, (ssymbol)(ch - '5' + SR1_SYMBOL_5));
		}
		if ('0' == ch)
		{
			return SCodeRS(SR1_NUM, SR1_SYMBOL_0);
		}

		return SCodeRS((sregion)SRERROR_NUM, 0);
	}
}