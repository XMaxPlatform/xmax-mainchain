/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <cstdarg>
#include <iostream>
#include <pro/log/debug.hpp>
#include <pro/log/log.hpp>


namespace pro
{
	static const string LineCode = "\r\n";

	static string LogFileName;// = "home:game.log";
	static const string string_log = "log: ";
	static const string string_warning = "warning: ";
	static const string string_error = "error:";


	void LogMessage(loglevel level, const string& msg)
	{
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
		std::cout << u8 << std::endl;
#if _Platform_Target == _Platform_Windows
		debug_output(u8.c_str());
#endif // WIN32
	}

}