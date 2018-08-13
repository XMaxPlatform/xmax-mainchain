/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <iostream>
#include <pro/log/debug.hpp>
#include <pro/log/console_logger.hpp>

namespace pro
{
	void ConsoleLogger::LogMessage(LogLevel level, const string& msg)
	{
		std::cout << msg << std::endl;

#if _Platform_Target == _Platform_Windows
		debug_output(msg.c_str());
#endif // WIN32
	}
}