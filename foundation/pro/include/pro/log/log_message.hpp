/**
*  @file
*  @copyright defined in xmax/LICENSE
*/

#pragma once
#include <string>

namespace pro {

	class LogMessage {
	public:
		LogMessage(const std::string& msg_content);
		~LogMessage() {}

		std::string GetMessage() const {
			return msg_content_;
		}

	private:
		std::string msg_content_;


	};
}