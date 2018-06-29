/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/scode/shortcode.hpp>

namespace pro
{
	typedef uint64_t NameCode;

	static constexpr int32_t NAME_CODE_BITS = sizeof(NameCode) * 8;
	static constexpr int32_t SCODE_R_0_BITS = 5;
	static constexpr int32_t SCODE_R_1_BITS = 8;

	static constexpr NameCode HEAD_MARK_R_0 = ((NameCode)0x1f) << (NAME_CODE_BITS - SCODE_R_0_BITS);
	static constexpr NameCode HEAD_MARK_R_1 = ((NameCode)0x07) << (NAME_CODE_BITS - SCODE_R_1_BITS);

	struct ShortName
	{
	public:
		ShortName()
			: code_(0)
		{

		}
		ShortName(NameCode c)
			: code_(c)
		{

		}
		inline NameCode Code() const
		{
			return code_;
		}

		bool operator == (const ShortName& rf)
		{
			return code_ == rf.code_;
		}
		bool operator != (const ShortName& rf)
		{
			return code_ != rf.code_;
		}
		bool operator > (const ShortName& rf)
		{
			return code_ > rf.code_;
		}
		bool operator < (const ShortName& rf)
		{
			return code_ < rf.code_;
		}
		bool operator >= (const ShortName& rf)
		{
			return code_ >= rf.code_;
		}
		bool operator <= (const ShortName& rf)
		{
			return code_ <= rf.code_;
		}
	private:
		NameCode code_;
	};

	static constexpr NameCode StringToShortName(const char* str)
	{
		NameCode namecode = 0;
		const char* ptr = str;
		int32_t remains = NAME_CODE_BITS;
		while (*ptr != 0)
		{
			char ch = *ptr;
			SCodeRS rs = GetSCodeRS(ch);
			if (SR0_NUM == rs.region)
			{
				if (remains < SCODE_R_0_BITS)
				{
					break;
				}
				remains -= SCODE_R_0_BITS;
				NameCode mark = ((NameCode)rs.symbol) << remains;
				namecode = (namecode | mark);
			}
			else if (SR1_NUM == rs.region)
			{
				if (remains < SCODE_R_1_BITS)
				{
					break;
				}
				remains -= SCODE_R_1_BITS;
				NameCode mark = ((NameCode)rs.symbol) << remains;
				namecode = (namecode | mark);
			}
			else
			{
				// illegal char.
			}
			++ptr;
		}
		return namecode;
	}

	static string ShortCodeToString(NameCode code)
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

			if (remains <  SCODE_R_1_BITS)
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

	static string ShortNameToString(ShortName name)
	{
		return ShortCodeToString(name.Code());
	}

#define SN(X) ShortName(pro::StringToShortName(#X))

}