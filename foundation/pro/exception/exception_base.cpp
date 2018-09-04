
#include <pro/utils/string_utils.hpp>
#include <pro/exception/exception_base.hpp>

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
	};

	Exception::~Exception() throw()
	{

	}

	Exception::Exception(const string& _description, const string& _source)
		: impl_{new ExceptionImpl()}		
	{
		impl_->line = 0;
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = _description;
		impl_->source = _source;
	}

	Exception::Exception(string&& _description, string&& _source)
		: impl_{ new ExceptionImpl() }		
	{
		impl_->line = 0;
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = _description;
		impl_->source = _source;
	}

	Exception::Exception(const string& _description, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }		
	{
		
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = _description;
		impl_->file = _file;
		impl_->line = _line;
	}

	Exception::Exception(string&& _description, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }		

	{
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = std::forward<string>(_description);
		impl_->file = _file;
		impl_->line = _line;
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
	}
	Exception::Exception(int type_, const string& _description, const string& _source, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }	
	{
		impl_->line = _line;
		impl_->type = static_cast<ExceptionType>(type_);
		impl_->title = title_;
		impl_->description = _description;
		impl_->source = _source;
		impl_->file = _file;		
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
	}
	Exception::Exception(int type_, const string& _description, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }	
	{
		impl_->line = _line;
		impl_->type = static_cast<ExceptionType>(type_);
		impl_->title = tile_;
		impl_->description = _description;
		impl_->file = _file;
	}
	Exception::Exception(int type_, string&& _description, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }		
	{
		impl_->line = _line;
		impl_->type = static_cast<ExceptionType>(type_);
		impl_->title = tile_;
		impl_->description = std::forward<string>(_description);
		impl_->file = _file;
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

    const string& Exception::GetFullDescription() const
	{
		if (0 == full_desc_.size())
		{	
			if( impl_->line > 0 )
			{
				if (source_.size())
				{
					utils::Sprintf(full_desc_, "EXCEPTION(%d:%s): \"%s\" in %s at '%s(line, %d)'",
						impl_->type, impl_->title.c_str(), impl_->description.c_str(), impl_->source.c_str(), impl_->file.c_str(), impl_->line);
				}
				else
				{
					utils::Sprintf(full_desc_, "EXCEPTION(%d:%s): \"%s\" at '%s(line, %d)'",
						impl_->type, impl_->title.c_str(), impl_->description.c_str(), impl_->file.c_str(), impl_->line);
				}
			
			}
			else
			{
				if (source_.size())
				{
					utils::Sprintf(full_desc_, "EXCEPTION(%d:%s): \"%s\" in %s", impl_->type, impl_->title.c_str(), impl_->description.c_str(), impl_->source.c_str());
				}
				else
				{
					utils::Sprintf(full_desc_, "EXCEPTION(%d:%s): \"%s\".", impl_->type, impl_->title.c_str(), impl_->description.c_str());
				}

			}
		}

		return full_desc_;
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