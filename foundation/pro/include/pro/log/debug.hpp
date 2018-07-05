/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/exception/exceptions.hpp>

namespace pro
{

	/**
	* ouput a msg to console
	* @param[in]	char*	content of msg
	*/
	void _debug_output(const char* ouput);
	/**
	* ouput a msg to console in a line
	* @param[in]	char*	content of msg
	*/
	void _debug_output_line(const char* ouput);


#define debug_output(x) _debug_output(x);
#define debug_output_line(x) _debug_output_line(x)


#define PRO_ASSERT(condition, fmt, ...) if(!condition) PRO_EXCEPT_WITH_DESC(pro::AssertException,  STRING_FORMAT(fmt, __VA_ARGS__))

}