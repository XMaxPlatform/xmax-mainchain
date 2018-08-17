/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/exception/exception_base.hpp>

namespace pro
{

#define PRO_EXCEPTION_TYPE(_ClassName, _Type)\
	class _ClassName : public pro::Exception\
	{\
	public:\
		_ClassName(const pro::string& desc_, const pro::string& src_, const char* file_, long line_) \
		: pro::Exception(_Type, desc_, src_, #_ClassName, file_, line_) {\
		}\
		_ClassName(const pro::string& desc_, const char* file_, long line_) \
		: pro::Exception(_Type, desc_, #_ClassName, file_, line_) {\
		}\
	}

	PRO_EXCEPTION_TYPE(AssertException, ExceptionType::EXT_ASSERT_ERROR);

	PRO_EXCEPTION_TYPE(SerializeException, ExceptionType::EXT_SERIALIZE_ERROR);

	PRO_EXCEPTION_TYPE(FormatException, ExceptionType::EXT_FORMAT_ERROR);

	PRO_EXCEPTION_TYPE(MemoryException, ExceptionType::EXT_MEMORY_ERROR);


#define PRO_EXCEPT_WITH_DESC(_type, desc) throw _type(desc, __FILE__, __LINE__)

}
