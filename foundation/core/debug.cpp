/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#if _Xmax_Platform == _Xmax_Platform_Windows
#include <Windows.h>
#   pragma warning (disable : 4996)
#   pragma warning (disable : 4717)
#endif // _DEBUG


#include "debug.hpp"



#if _Xmax_Platform == _Xmax_Platform_Windows
#	define  _DEBUG_OUTPUTA(x) OutputDebugStringA(x)
#else
#	define	_DEBUG_OUTPUTA(x)
#endif // WIN32


namespace Xmax
{

#if _Xmax_Platform == _Xmax_Platform_Windows
	static const char line_code[] = "\n";
#endif

	void _debug_output(const char* ouput)
	{
		_DEBUG_OUTPUTA(ouput);
	}

	void _debug_output_line(const char* ouput)
	{
		_DEBUG_OUTPUTA(ouput);
		_DEBUG_OUTPUTA(line_code);
	}

}