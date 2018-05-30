/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <build.hpp>

namespace Xmax
{


	void _debug_output(const char* ouput);
	void _debug_output_line(const char* ouput);


#define debug_output(x) _debug_output(x);
#define debug_output_line(x) _debug_output_line(x)

	
}