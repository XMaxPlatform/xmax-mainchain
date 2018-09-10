/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#include <vector>
#include <memory>
#include <iostream>

#include <pro/utils/time_utils.hpp>
#include <pro/log/console_logger.hpp>
#include <pro/log/log_system.hpp>

namespace pro
{
	static const string LineCode = "\r\n";

	static string LogFileName;// = "home:game.log";
	static const string string_log =		"log    ";
	static const string string_warning =	"warn   ";
	static const string string_error =		"error  ";


	class LogSystemImpl
	{
	public:
		typedef std::vector<LoggerPtr> Loggers;

		void Append(LoggerPtr logger)
		{
			if (!Contain(logger.get()))
			{
				loggers_.push_back(logger);
			}
		}
		void Remove(LoggerPtr logger)
		{
			auto it = Find(logger.get());
			if (it != loggers_.end())
			{
				loggers_.erase(it);
			}
		}

		Loggers::const_iterator Find(ILogger* logger) const
		{
			for (Loggers::const_iterator it = loggers_.begin(); it != loggers_.end(); ++ it)
			{
				if ((*it).get() == logger)
				{
					return it;
				}
			}
			return loggers_.end();
		}

		bool Contain(ILogger* logger) const
		{
			return Find(logger) != loggers_.end();
		}

		void LogMessage(LogLevel level, const string& msg)
		{
			std::stringstream strbuff;
			switch (level)
			{
			case LogLevel::Info:
			{
				strbuff << string_log;
				break;
			}
			case LogLevel::Warnning:
			{
				strbuff << string_warning;
				break;
			}
			case LogLevel::Error:
			{
				strbuff << string_error;
				break;
			}
			default:
			{
				break;
			}
			}
			strbuff << utils::TimeNowDateMSString()<<"] ";

			strbuff << msg;
			strbuff << LineCode;
			
			for ( auto& logger : loggers_)
			{
				logger->LogMessage(level, strbuff.str());
			}
		}


		Loggers loggers_;
	};


	LogSystem::LogSystem()
		: impl_(std::make_unique<LogSystemImpl>())
	{
		Append<ConsoleLogger>();
	}
	LogSystem::~LogSystem()
	{

	}
	void LogSystem::Append(LoggerPtr logger)
	{
		impl_->Append(logger);
	}
	void LogSystem::Remove(LoggerPtr logger)
	{
		impl_->Remove(logger);
	}

	void LogSystem::LogMessage(LogLevel level, const string& msg)
	{
		impl_->LogMessage(level, msg);
	}

}