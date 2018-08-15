#include "pro/log/log_message.hpp"


namespace pro {

	/*
	* LogContext class implementation
	*/
	//--------------------------------------------------
	LogContext::LogContext()
	{

	}


	//--------------------------------------------------
	LogContext::LogContext(LogLevel level):log_level_(level)
	{

	}


	//--------------------------------------------------
	LogContext::LogContext(LogLevel level, const char* file, unsigned int line, const char* method):
		log_level_(level), file_(file), line_(line), method_(method)

	{

	}


	//--------------------------------------------------
	LogContext::~LogContext()
	{

	}

	/*
	* LogMessage class implementation
	*/
	//--------------------------------------------------
	LogMessage::LogMessage(const std::string& msg_content, LogLevel level):
		msg_content_(msg_content),
		log_context_(level)
	{

	}


	

}