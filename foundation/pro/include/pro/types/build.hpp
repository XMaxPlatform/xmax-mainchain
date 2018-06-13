/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#define _Platform_Unknown 0
#define _Platform_Windows 1
#define _Platform_Linux 2
#define _Platform_MacOS 3

#define _Compiler_Unknown 0
#define _Compiler_MSVC 1
#define _Compiler_CLANG 2

#if defined(WIN32)
#	define _Platform_Target _Platform_Windows
#endif

#if defined( _MSC_VER )
#   define _Project_Compiler _Compiler_MSVC
#endif


// GCC settings
#if defined __GNUC__
#define __cdecl
#define __forceinline inline
#endif




#if _Platform_Target == _Platform_Windows
#ifndef _WIN32_WINNT
#	define _WIN32_WINNT NTDDI_WIN7
#endif
#endif 

namespace pro
{
}