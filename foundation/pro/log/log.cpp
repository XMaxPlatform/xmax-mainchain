/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <cstdarg>
#include <iostream>
#include <pro/log/log_system.hpp>
#include <pro/log/log.hpp>


namespace pro
{
	void LogMessage(loglevel level, const string& msg)
	{
		LogSystem::GetInstance().LogMessage(level, msg);
	}



}