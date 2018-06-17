/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <pro/types/generictypes.hpp>

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
	void _log_message(loglevel level, const string& msg);
	/**
	* output a log to console 
	* @param[in]	loglevel	level of log
	* @param[in]	const char*		content of log
	*/
	void _log_message(loglevel level, const char* format, ...);
	/**
	* save the log to a file
	* @param[in]	stirng	 name of a file
	*/
	void _set_logfilename(const string& name);


#define iwarnning(fmt, ...) pro::_log_message(pro::loglevel(pro::loglevel::Warnning), fmt, __VA_ARGS__)

#define ilog(fmt, ...) pro::_log_message(pro::loglevel(pro::loglevel::Info), fmt, __VA_ARGS__)

//#define ierror(fmt, ...) _xmax_error(__FILE__, __LINE__, fmt, __VA_ARGS__)


}