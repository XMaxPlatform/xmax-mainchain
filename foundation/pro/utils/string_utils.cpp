
#include <cstdarg>
#include <pro/utils/string_utils.h>

#define __char_cache_size 1024
namespace pro
{
	namespace utils
	{
		void Format(string& buff, const char* format, ...)
		{
			char msg[__char_cache_size] = { 0 };
			va_list arg_ptr;
			va_start(arg_ptr, format);
			int nWrittenBytes = vsprintf_s(msg, __char_cache_size - 1, format, arg_ptr);
			va_end(arg_ptr);
			buff = msg;
		}
	}
}