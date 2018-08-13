/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <pro/log/log_base.hpp>
#include <pro/utils/string_utils.hpp>
#include <pro/exception/exceptions.hpp>

namespace pro
{
	/**
	* output a log to console
	* @param[in]	loglevel	level of log
	* @param[in]	string		content of log
	*/
	void LogMessage(LogLevel level, const string& msg);

	/**
	* save the log to a file
	* @param[in]	stirng	 name of a file
	*/
	void _set_logfilename(const string& name);

}

#define EXCEPT_LOG_ERROR(desc) PRO_EXCEPT_WITH_DESC(pro::AssertException, desc);

/**
* format warn log.
*/
#define WarnSprintf(fmt, ...) pro::LogMessage(pro::LogLevel(pro::LogLevel::Warnning), pro::utils::Sprintf(fmt, __VA_ARGS__))
/**
* format info log.
*/
#define LogSprintf(fmt, ...) pro::LogMessage(pro::LogLevel(pro::LogLevel::Info), pro::utils::Sprintf(fmt, __VA_ARGS__))
/**
* format error log.
*/
#define ErrorSprintf(fmt, ...) \
{\
	const pro::string desc = pro::utils::Sprintf(fmt, __VA_ARGS__);\
	pro::LogMessage(pro::LogLevel(pro::LogLevel::Error), desc);\
	EXCEPT_LOG_ERROR(desc); \
}



/**
* warn log.
*/
#define Warnf(fmt, ...) pro::LogMessage(pro::LogLevel(pro::LogLevel::Warnning), STRING_FORMAT(fmt, __VA_ARGS__))
/**
* info log.
*/
#define Logf(fmt, ...) pro::LogMessage(pro::LogLevel(pro::LogLevel::Info), STRING_FORMAT(fmt, __VA_ARGS__))

/**
* error log.
*/
#define Errorf(fmt, ...) \
{\
	const pro::string desc = STRING_FORMAT(fmt, __VA_ARGS__);\
	pro::LogMessage(pro::LogLevel(pro::LogLevel::Error), desc);\
	EXCEPT_LOG_ERROR(desc);\
}

