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


	class ILogger
	{
	public:
		virtual void LogMessage(loglevel level, const string& msg) = 0;


		~ILogger() {}
	private:
	};

	using LoggerPtr = std::shared_ptr<ILogger>;

}