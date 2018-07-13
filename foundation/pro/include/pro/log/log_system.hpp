/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <pro/log/log_base.hpp>
#include <pro/types/generictypes.hpp>
#include <pro/utils/singleton.hpp>
#include <memory>

namespace pro
{

	class LogSystemImpl;


	class LogSystem : public Singleton<LogSystem>
	{
	public:
		DECLARE_USE_SINGLETON(LogSystem)
		/**
		* output a log to console
		* @param[in]	loglevel	level of log
		* @param[in]	string		content of log
		*/
		void LogMessage(loglevel level, const string& msg);


		void Append(LoggerPtr logger);
		void Remove(LoggerPtr logger);


		template<typename T>
		std::shared_ptr<T> Append()
		{
			auto ptr = std::make_shared<T>();
			Append(ptr);
			return ptr;
		}
		//LogSystem();
	private:
		std::unique_ptr<LogSystemImpl> impl_;
	};
}