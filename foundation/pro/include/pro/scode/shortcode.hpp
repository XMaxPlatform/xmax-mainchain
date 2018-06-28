/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>

namespace pro
{
	typedef char scode;
	typedef uint32_t sencode;
	struct SCodeDesc
	{
	public:
		scode ch;
		sencode encode;
		constexpr SCodeDesc()
			: ch(0)
			, encode(0)
		{

		}
		constexpr SCodeDesc(char _ch, sencode _encode)
			: ch(_ch)
			, encode(_encode)
		{

		}
	};

	static constexpr scode BASE_SCODE_SHEET[] =
	{ 
		{(scode)0}//0
		,{ 'a' },{ 'b' },{ 'c' },{ 'd' },{ 'e' },{ 'f' },{ 'g' }	//0x01-0x07
		,{ 'h' },{ 'i' },{ 'j' },{ 'k' },{ 'l' },{ 'm' },{ 'n' }	//0x08-0x0E
		,{ 'o' },{ 'p' },{ 'q' },{ 'r' },{ 's' },{ 't' }			//0x0F-0x14
		,{ 'u' },{ 'v' },{ 'w' },{ 'x' },{ 'y' },{ 'z' }			//0x15-0x1A
		,{ '@' },{ '.' },{ '-' },{ '(' },{ ')' }					//0x1B-0x1F					
		,{ '0' },{ '1' },{ '2' },{ '3' },{ '4' }					//0x20-0x24
		,{ '5' },{ '6' },{ '7' },{ '8' },{ '9' }					//0x25-0x29
	};
	static constexpr sencode BASE_SCODE_NUM = 41;// 0x29;

	static constexpr scode GetSCode(sencode code)
	{		
		if (code <= 0 || code > BASE_SCODE_NUM)
		{
			return (scode)0;
		}
		return BASE_SCODE_SHEET[code];
	}

	static constexpr SCodeDesc GetSCodeDesc(sencode code)
	{

		return SCodeDesc(GetSCode(code), code);
	}
}