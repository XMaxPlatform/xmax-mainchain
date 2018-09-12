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

	class LogMessageImpl {
	public:
		LogMessageImpl() {}

		LogMessageImpl(const std::string& content, const LogContext& context) :
			msg_content(content),
			log_context(context)
		{

		}



		std::string msg_content;
		LogContext log_context;
	};

	//--------------------------------------------------
	std::string LogContext::ToString() const
	{		
		return file_ + ":" + std::to_string(line_) + " " + method_;
	}

	/*
	* LogMessage class implementation
	*/
	//--------------------------------------------------
	LogMessage::LogMessage(const std::string& msg_content, LogLevel level)
	{
		impl_ = std::make_shared<LogMessageImpl>(msg_content, LogContext{level});
	}

		

	//--------------------------------------------------
	LogMessage::LogMessage(const std::string& msg_contet, LogContext context):
		impl_(std::make_shared<LogMessageImpl>(msg_contet, context))
	{

	}


	//--------------------------------------------------
	LogMessage::LogMessage():
		impl_(std::make_shared<LogMessageImpl>())
	{

	}


	//--------------------------------------------------
	LogMessage::~LogMessage()
	{
		impl_.reset();
	}

	//--------------------------------------------------
	std::string LogMessage::GetMessage() const
	{

		return impl_->msg_content;

	}

}