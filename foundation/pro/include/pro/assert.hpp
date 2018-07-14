/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/exception/exceptions.hpp>


#define PRO_ASSERT(condition, fmt, ...) if(!condition) PRO_EXCEPT_WITH_DESC(pro::AssertException,  STRING_FORMAT(fmt, __VA_ARGS__))

namespace pro
{

}