/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <generictypes.hpp>

namespace Xmax
{
	using Xmax::Generictypes::string;

	enum loglevel : int
	{
		Info = 0,
		Warnning = 100,
		Error = 200,
	};


	void _log_message(loglevel level, const string& msg);

	//void _log_message(loglevel level, const std::stringstream& sstream);

	void _log_message(loglevel level, const char* format, ...);

	//void _xmax_error(const char* file, long line, const char* format, ...);

	void _set_logfilename(const string& name);


#define iwarnning(fmt, ...) _log_message(Xmax::loglevel(Xmax::loglevel::Warnning), fmt, __VA_ARGS__)

#define ilog(fmt, ...) _log_message(Xmax::loglevel(Xmax::loglevel::Info), fmt, __VA_ARGS__)

//#define ierror(fmt, ...) _xmax_error(__FILE__, __LINE__, fmt, __VA_ARGS__)


}