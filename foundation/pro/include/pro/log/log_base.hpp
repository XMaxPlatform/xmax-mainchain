/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <pro/types/generictypes.hpp>
#include <memory>

namespace pro
{
	/**
	* log level enum
	*/
	enum loglevel : int
	{
		Info = 0,/*!< Info level. */  
		Warnning = 100,/*!< Warnning level. */  
		Error = 200,/*!< Error level. */  
	};


	class ILogger
	{
	public:
		/**
		* Log a message.
		* @param[in] level log level type.
		* @param[in] msg the message.
		*/
		virtual void LogMessage(loglevel level, const string& msg) = 0;


		~ILogger() {}
	private:
	};
	/**
	* the ptr of ILogger.
	*/
	using LoggerPtr = std::shared_ptr<ILogger>;

}