#include "pro/log/log_message.hpp"


namespace pro {

	//--------------------------------------------------
	LogMessage::LogMessage(const std::string& msg_content, LogLevel level):
		msg_content_(msg_content),
		log_level_(level)
	{

	}

}