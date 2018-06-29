/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/scode/shortname.hpp>

namespace pro
{

	ShortName::ShortName()
		: namecode_(0)
	{

	}

	ShortName::ShortName(NameCode c)
		: namecode_(c)
	{

	}
	ShortName ShortName::Create(NameCode c)
	{
		return ShortName(c);
	}

	string ShortName::ToString() const
	{
		return ShortName::ToString(namecode_);
	}
	string ShortName::ToString(NameCode code)
	{
		int32_t remains = NAME_CODE_BITS;
		string name;

		NameCode namecode = code;
		NameCode tmpcode = 0;
		while (remains >= SCODE_R_0_BITS)
		{
			tmpcode = ((NameCode)namecode) & HEAD_MARK_R_0;

			if (tmpcode != 0)
			{
				// region 0
				remains -= SCODE_R_0_BITS;
				sglyph ch = GetSGlyph(SR0_NUM, tmpcode >> (NAME_CODE_BITS - SCODE_R_0_BITS));
				name.push_back(ch);
				namecode = namecode << SCODE_R_0_BITS;
				continue;
			}

			if (remains < SCODE_R_1_BITS)
			{
				break;
			}
			tmpcode = ((NameCode)namecode) & HEAD_MARK_R_1;
			if (tmpcode != 0)
			{
				// region 1
				remains -= SCODE_R_1_BITS;
				sglyph ch = GetSGlyph(SR1_NUM, tmpcode >> (NAME_CODE_BITS - SCODE_R_1_BITS));
				name.push_back(ch);
				namecode = namecode << SCODE_R_1_BITS;
				continue;
			}
			// name end.
			break;
		}
		return name;
	}

	string ShortName::ToString(ShortName name)
	{
		return ToString(name.Code());
	}

	uint32_t ShortName::GlyphBitLength(sglyph gl)
	{
		auto desc = GetSCodeRS(gl);

		switch (desc.region)
		{
		case SR0_NUM:
		{
			return SCODE_R_0_BITS;
		}
		case SR1_NUM:
		{
			return SCODE_R_1_BITS;
		}
		default:
			break;
		}

		return 0;
	}

	uint32_t ShortName::NameLengthWithBit(const char* str)
	{
		uint32_t len = 0;

		while (0 != *str)
		{
			len += GlyphBitLength(static_cast<sglyph>(*str));
			++str;
		}

		return len;
	}

	bool ShortName::LegalName(const char* str)
	{

		uint32_t totallen = 0;

		while (0 != *str)
		{
			uint32_t len = GlyphBitLength(static_cast<sglyph>(*str));
			if (0 == len)
			{
				return false;
			}
			totallen += len;
			++str;
		}

		if (totallen > NAME_CODE_BITS)
		{
			return false;
		}

		return true;
	}
}