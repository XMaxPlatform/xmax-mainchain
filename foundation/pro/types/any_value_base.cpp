/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/utils/string_utils.hpp>
#include <pro/types/any_value_base.hpp>


#define CONVERT_CASE(_type, _code) \
		case _code: {\
			if constexpr (AnyType::IsSimpleType<_code>())\
				utils::ToString(str, CastImpl<_type, _code>(v));\
			else\
				AnyType::AsContainer(v)->ToString(str);\
			break;\
		}

namespace pro
{

	void IAnyContainer::ToString(string& str) const
	{
		size_t addr = reinterpret_cast<size_t>(GetValuePtr());
		string addrs;
		utils::ToString(addrs, addr);
		str = "addr=" + addrs;
	}

	void AnyType::ToString(string& str, void* v, AnyType::Code code)
	{


		switch (code)
		{
			IMPLEMENT_ANY_TYPE_BIND(CONVERT_CASE)
		default:
			break;
		}
	}
}