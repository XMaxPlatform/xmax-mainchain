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
		ShortName();

		inline NameCode Code() const
		{
			return namecode_;
		}

		inline bool operator == (const ShortName& rf)
		{
			return namecode_ == rf.namecode_;
		}

		friend bool operator == (const ShortName& lhs, const ShortName& rhs)
		{
			return lhs.namecode_ == rhs.namecode_;
		}

		inline bool operator != (const ShortName& rf)
		{
			return namecode_ != rf.namecode_;
		}
		inline bool operator > (const ShortName& rf)
		{
			return namecode_ > rf.namecode_;
		}
		inline bool operator < (const ShortName& rf)
		{
			return namecode_ < rf.namecode_;
		}
		inline bool operator >= (const ShortName& rf)
		{
			return namecode_ >= rf.namecode_;
		}
		inline bool operator <= (const ShortName& rf)
		{
			return namecode_ <= rf.namecode_;
		}

		string ToString() const;

		static ShortName Create(NameCode c);

		static string ToString(NameCode code);

		static string ToString(ShortName name);

		static uint32_t GlyphBitLength(sglyph gl);

		static uint32_t NameLengthWithBit(const char* str);

		static bool LegalName(const char* str);

		static constexpr NameCode ToShortName(const char* str)
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
	private:
		ShortName(NameCode c);

		NameCode namecode_;
	};



#define SN(X) pro::ShortName::Create(pro::ShortName::ToShortName(#X))

}