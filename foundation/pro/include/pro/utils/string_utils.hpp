/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <pro/types/generictypes.hpp>

namespace pro
{
	class AnyObject;

	namespace utils
	{
		void PrintFormat(string& buff, const char* format, ...);

		string StringFormat(const string& format, const AnyObject& args);


		template <typename T>
		static void ToString(string& str, const T& v)
		{
			str = std::to_string<T>(v);
		}
	}
}


