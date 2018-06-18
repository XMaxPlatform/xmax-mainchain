/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>

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

	
}