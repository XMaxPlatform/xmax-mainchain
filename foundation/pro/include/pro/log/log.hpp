/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <pro/types/generictypes.hpp>
#include <pro/utils/string_utils.hpp>

namespace pro
{
	/**
	* log level enum
	*/
	enum loglevel : int
	{
		Info = 0,
		Warnning = 100,
		Error = 200,
	};
	/**
	* output a log to console
	* @param[in]	loglevel	level of log
	* @param[in]	string		content of log
	*/
	void LogMessage(loglevel level, const string& msg);


	void _log_message(loglevel level, const string& format, const AnyObject& args);
	/**
	* save the log to a file
	* @param[in]	stirng	 name of a file
	*/
	void _set_logfilename(const string& name);

}

#define WarnSprintf(fmt, ...) pro::LogMessage(pro::loglevel(pro::loglevel::Warnning), pro::utils::Sprintf(fmt, __VA_ARGS__))

#define LogSprintf(fmt, ...) pro::LogMessage(pro::loglevel(pro::loglevel::Info), pro::utils::Sprintf(fmt, __VA_ARGS__))

#define ErrorSprintf(fmt, ...) pro::LogMessage(pro::loglevel(pro::loglevel::Error), pro::utils::Sprintf(fmt, __VA_ARGS__))



#define Warnf(fmt, ...) pro::LogMessage(pro::loglevel(pro::loglevel::Warnning), STRING_FORMAT(fmt, __VA_ARGS__))

#define Logf(fmt, ...) pro::LogMessage(pro::loglevel(pro::loglevel::Info), STRING_FORMAT(fmt, __VA_ARGS__))

#define Errorf(fmt, ...) pro::LogMessage(pro::loglevel(pro::loglevel::Error), STRING_FORMAT(fmt, __VA_ARGS__))
