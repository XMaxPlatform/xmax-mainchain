/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

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

	/**
	* ouput a msg to console
	*/
#define debug_output(x) _debug_output(x);
	/**
	* ouput a msg with line code to console
	*/
#define debug_output_line(x) _debug_output_line(x)
}