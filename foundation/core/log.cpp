/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <cstdarg>
#include <iostream>
#include "debug.hpp"
#include "log.hpp"


namespace Xmax
{
	static const string LineCode = "\r\n";

	static string LogFileName;// = "home:game.log";
	static const string string_log = "log: ";
	static const string string_warning = "warning: ";
	static const string string_error = "error:";


	void _log_message(loglevel level, const string& msg)
	{
		std::cout << msg << std::endl;


#if _Xmax_Platform == _Xmax_Platform_Windows
		string u8;
		switch (level)
		{
		case loglevel::Info:
		{
			u8 = string_log;
			break;
		}
		case loglevel::Warnning:
		{
			u8 = string_warning;
			break;
		}
		case loglevel::Error:
		{
			u8 = string_error;
			break;
		}
		default:
		{
			break;
		}
		}

		u8 += msg;
		u8 += LineCode;
		debug_output(u8.c_str());
#endif // WIN32
	}


	//void _log_message(loglevel level, const std::stringstream& sstream)
	//{
	//	_log_message(level, sstream.str());
	//}

	void _log_message(loglevel level, const char* format, ...)
	{
		char msg[1024] = { 0 };
		va_list arg_ptr;
		va_start(arg_ptr, format);
		int nWrittenBytes = vsprintf_s(msg, 1024, format, arg_ptr);
		va_end(arg_ptr);

		string u8 = msg;
		_log_message(level, u8);
	}


	//void _xmax_error(const char* file, long line, const char* format, ...)
	//{
	//	char msg[1024] = { 0 };
	//	va_list arg_ptr;
	//	va_start(arg_ptr, format);
	//	int nWrittenBytes = vsprintf_s(msg, 1024, format, arg_ptr);
	//	va_end(arg_ptr);
	//	string u8 = msg;
	//	_log_message(loglevel::Error, u8);

	//	throw Exception("error", msg, file, line);
	//}

}