/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pro/types/build.hpp>
#include <filesystem>

namespace pro
{
#if _Project_Compiler == _Compiler_MSVC
#	if _MSC_VER < 1914
	namespace fs = std::experimental::filesystem;
#	else
	namespace fs = std::filesystem;
#	endif
#else
	namespace fs = std::filesystem;
#endif
}