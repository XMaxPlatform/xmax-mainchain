/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <string>
#include <pro/log/log_base.hpp>

namespace pro {

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