
#include <pro/utils/string_utils.hpp>
#include <pro/exception/exception_base.hpp>
#include <pro/log/log_message.hpp>

namespace pro
{
	class ExceptionImpl {
	public:
		long line{ 0 };
		ExceptionType type{ EXT_UNDEF_TYPE };
		string title;
		string description;
		string source;
		string file;
		string full_desc;
		LogMessage log_message;

	public:
		//Constructors
		ExceptionImpl() {}
		ExceptionImpl(const LogMessage& message);
		ExceptionImpl(LogMessage&& message);


		std::string FullDescription() const;
	};


	//--------------------------------------------------
	ExceptionImpl::ExceptionImpl(const LogMessage& message):
		log_message(message)
	{

	}


	//--------------------------------------------------
	ExceptionImpl::ExceptionImpl(LogMessage&& message):
		log_message(std::move(message))
	{

	}

	//--------------------------------------------------
	std::string ExceptionImpl::FullDescription() const
	{
		std::string full_desc;

		if (0 == full_desc.size())
		{
			if (line > 0)
			{
				if (source.size())
				{
					utils::Sprintf(full_desc, "EXCEPTION(%d:%s): \"%s\" in %s at '%s(line, %d)'",
						type, title.c_str(), description.c_str(), source.c_str(), file.c_str(), line);
				}
				else
				{
					utils::Sprintf(full_desc, "EXCEPTION(%d:%s): \"%s\" at '%s(line, %d)'",
						type, title.c_str(), description.c_str(), file.c_str(), line);
				}

			}
			else
			{
				if (source.size())
				{
					utils::Sprintf(full_desc, "EXCEPTION(%d:%s): \"%s\" in %s", type, title.c_str(), description.c_str(), source.c_str());
				}
				else
				{
					utils::Sprintf(full_desc, "EXCEPTION(%d:%s): \"%s\".", type, title.c_str(), description.c_str());
				}

			}
		}

		return full_desc;
	}

	Exception::~Exception() throw()
	{

	}

	Exception::Exception(std::string _description, const string _source):
		impl_(std::make_shared<ExceptionImpl>())

	{
		impl_->line = 0;
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = std::move(_description);
		impl_->source = std::move(_source);
		impl_->log_message = LogMessage(_description, LogContext(LogLevel::Error));
	}

	Exception::Exception(string _description, const char* _file, long _line):
		impl_(std::make_shared<ExceptionImpl>())
	{
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = std::move(_description);
		impl_->file = _file;
		impl_->line = _line;
		impl_->log_message = LogMessage(_description, LogContext(LogLevel::Error, _file, _line, ""));
	}
	
	Exception::Exception(const string& _description, const string& _source, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }		
		
	{
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = _description;
		impl_->source = _source;
		impl_->file = _file;
		impl_->line = _line;
		impl_->log_message = LogMessage(_description, LogContext(LogLevel::Error, _file, _line, ""));
	}
	Exception::Exception(string&& _description, string&& _source, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }		

	{
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = std::forward<string>(_description);
		impl_->source = std::forward<string>(_source);
		impl_->file = _file;
		impl_->line = _line;
		impl_->log_message = LogMessage(_description, LogContext(LogLevel::Error, _file, _line, ""));
	}
	Exception::Exception(int type_, const string& _description, const string& _source, const char* _title, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }	
	{
		impl_->line = _line;
		impl_->type = static_cast<ExceptionType>(type_);
		impl_->title = _title;
		impl_->description = _description;
		impl_->source = _source;
		impl_->file = _file;		
		impl_->log_message = LogMessage(_description, LogContext(LogLevel::Error, _file, _line, ""));
	}

	Exception::Exception(int type_, string&& _description, string&& _source, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }	
	{
		impl_->line = _line;
		impl_->type = static_cast<ExceptionType>(type_);
		impl_->title = std::forward<string>(tile_);
		impl_->description = std::forward<string>(_description);
		impl_->source = _source;
		impl_->file = _file;
		impl_->log_message = LogMessage(_description, LogContext(LogLevel::Error, _file, _line, ""));
	}
	Exception::Exception(int type_, const string& _description, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }	
	{
		impl_->line = _line;
		impl_->type = static_cast<ExceptionType>(type_);
		impl_->title = tile_;
		impl_->description = _description;
		impl_->file = _file;
		impl_->log_message = LogMessage(_description, LogContext(LogLevel::Error, _file, _line, ""));
	}
	Exception::Exception(int type_, string&& _description, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }		
	{
		impl_->line = _line;
		impl_->type = static_cast<ExceptionType>(type_);
		impl_->title = tile_;
		impl_->description = std::forward<string>(_description);
		impl_->file = _file;
		impl_->log_message = LogMessage(_description, LogContext(LogLevel::Error, _file, _line, ""));
	}


	Exception::Exception(const Exception& rhs)
		: impl_{ new ExceptionImpl() }	
	{
		impl_ = rhs.impl_;
	}

	Exception::Exception(Exception&& rhs)
		: impl_{ new ExceptionImpl() }	
	{
		impl_ = std::move(rhs.impl_);
	}


	Exception& Exception::operator = (const Exception& rhs)
	{
		impl_ = rhs.impl_;

		return *this;
	}

	Exception& Exception::operator = (Exception&& rhs)
	{

		impl_ = rhs.impl_;

		return *this;
	}

    string Exception::GetFullDescription() const
	{
		return impl_->FullDescription();
	}

    int Exception::GetType(void) const throw()
	{
		return impl_->type;
	}

    const string &Exception::GetSource() const 
	{ 
		return impl_->source;
	}

    const string &Exception::GetFile() const 
	{ 
		return impl_->file;
	}

    long Exception::GetLine() const 
	{ 
		return impl_->line; 
	}

	const string &Exception::GetDescription(void) const 
	{ 
		return impl_->description; 
	}

	const char* Exception::what() const throw() 
	{ 
		return GetFullDescription().c_str(); 
	}
}