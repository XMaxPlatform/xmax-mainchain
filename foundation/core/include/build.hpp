/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#define _Xmax_Platform_Unknown 0
#define _Xmax_Platform_Windows 1
#define _Xmax_Platform_Linux 2
#define _Xmax_Platform_MacOS 3

#define _Xmax_Compiler_Unknown 0
#define _Xmax_Compiler_MSVC 1
#define _Xmax_Compiler_CLANG 2

#if defined(WIN32)
#	define _Xmax_Platform _Xmax_Platform_Windows
#endif

#if defined( _MSC_VER )
#   define _Xmax_Compiler _Xmax_Compiler_MSVC
#endif


// GCC settings
#if defined __GNUC__
#define __cdecl
#define __forceinline inline
#endif




#if _Xmax_Platform == _Xmax_Platform_Windows
#ifndef _WIN32_WINNT
#	define _WIN32_WINNT NTDDI_WIN7
#endif
#endif 

namespace Xmax
{
}