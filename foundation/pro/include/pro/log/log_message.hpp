/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <string>
#include <pro/log/log_base.hpp>

namespace pro {

	class LogContext {
	public:
		LogContext();
		LogContext(LogLevel level);
		LogContext(LogLevel level, const char* file, unsigned int line, const char* method);
		~LogContext();


	private:
		LogLevel log_level_{LogLevel::Info};
		std::string file_;
		unsigned int line_{ 0 };
		std::string method_;
	};


	class LogMessage {
	public:
		LogMessage(const std::string& msg_content, LogLevel level = LogLevel::Info);
		~LogMessage() {}

		std::string GetMessage() const {
			return msg_content_;
		}

	private:
		std::string msg_content_;
		LogLevel log_level_{ LogLevel::Info };


	};
}