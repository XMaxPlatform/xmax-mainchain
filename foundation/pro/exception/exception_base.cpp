
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
		, line_(0)
		, type_(EXT_UNDEF_TYPE)
		, title_("Exception")
		, description_(_description)
		, source_(_source)
	{
		impl_->line = 0;
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = _description;
		impl_->source = _source;
	}

	Exception::Exception(string&& _description, string&& _source)
		: impl_{ new ExceptionImpl() }
		, line_(0)
		, type_(EXT_UNDEF_TYPE)
		, title_("Exception")
		, description_(std::forward<string>(_description))
		, source_(std::forward<string>(_source))
	{
		impl_->line = 0;
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = _description;
		impl_->source = _source;
	}
	Exception::Exception(const string& _description, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }
		, type_(EXT_UNDEF_TYPE)
		, title_("Exception")
		, description_(_description)
		, file_(_file)
		, line_(_line)

	{
		
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = _description;
		impl_->file = _file;
		impl_->line = _line;
	}

	Exception::Exception(string&& _description, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }
		, type_(EXT_UNDEF_TYPE)
		, title_("Exception")
		, description_(std::forward<string>(_description))
		, file_(_file)
		, line_(_line)

	{
		impl_->type = EXT_UNDEF_TYPE;
		impl_->title = "Exception";
		impl_->description = std::forward<string>(_description);
		impl_->file = _file;
		impl_->line = _line;
	}
	Exception::Exception(const string& _description, const string& _source, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }
		, type_(EXT_UNDEF_TYPE)
		,title_("Exception")
		,description_(_description)
		,source_(_source)
		,file_(_file)
		,line_(_line)
		
	{
	}
	Exception::Exception(string&& _description, string&& _source, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }
		, type_(EXT_UNDEF_TYPE)
		, title_("Exception")
		, description_(std::forward<string>(_description))
		, source_(std::forward<string>(_source))
		, file_(_file)
		, line_(_line)

	{
	}
	Exception::Exception(int type_, const string& _description, const string& _source, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }
		, line_(_line)
		,type_(type_)
		,title_(tile_)
		,description_(_description)
		,source_(_source)
		,file_(_file)
	{

	}
	Exception::Exception(int type_, string&& _description, string&& _source, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }
		, line_(_line)
		, type_(type_)
		, title_(std::forward<string>(tile_))
		, description_(std::forward<string>(_description))
		, source_(_source)
		, file_(_file)
	{

	}
	Exception::Exception(int type_, const string& _description, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }
		, line_(_line)
		, type_(type_)
		, title_(tile_)
		, description_(_description)
		, file_(_file)
	{

	}
	Exception::Exception(int type_, string&& _description, const char* tile_, const char* _file, long _line)
		:impl_{ new ExceptionImpl() }
		, line_(_line)
		, type_(type_)
		, title_(tile_)
		, description_(std::forward<string>(_description))
		, file_(_file)
	{

	}


	Exception::Exception(const Exception& rhs)
		: impl_{ new ExceptionImpl() }
		, line_(rhs.line_),
		type_(rhs.type_), 
		title_(rhs.title_), 
		description_(rhs.description_), 
		source_(rhs.source_), 
		file_(rhs.file_)
	{
	}
	Exception::Exception(Exception&& rhs)
		: impl_{ new ExceptionImpl() }
		, line_(rhs.line_),
		type_(rhs.type_),
		title_(std::forward<string>(rhs.title_)),
		description_(std::forward<string>(rhs.description_)),
		source_(std::forward<string>(rhs.source_)),
		file_(std::forward<string>(rhs.file_))
	{
	}

	Exception& Exception::operator = (const Exception& rhs)
	{
		line_ = rhs.line_;
		type_ = rhs.type_;

		title_ = rhs.title_;
		description_ = rhs.description_;
		source_ = rhs.source_;
		file_ = rhs.file_;

		impl_ = rhs.impl_;

		return *this;
	}

	Exception& Exception::operator = (Exception&& rhs)
	{
		line_ = rhs.line_;
		type_ = rhs.type_;

		title_ = std::forward<string>(rhs.title_);
		description_ = std::forward<string>(rhs.description_);
		source_ = std::forward<string>(rhs.source_);
		file_ = std::forward<string>(rhs.file_);

		impl_ = rhs.impl_;

		return *this;
	}

    const string& Exception::GetFullDescription() const
	{
		if (0 == full_desc_.size())
		{	
			if( line_ > 0 )
			{
				if (source_.size())
				{
					utils::Sprintf(full_desc_, "EXCEPTION(%d:%s): \"%s\" in %s at '%s(line, %d)'",
						type_, title_.c_str(), description_.c_str(), source_.c_str(), file_.c_str(), line_);
				}
				else
				{
					utils::Sprintf(full_desc_, "EXCEPTION(%d:%s): \"%s\" at '%s(line, %d)'",
						type_, title_.c_str(), description_.c_str(), file_.c_str(), line_);
				}
			
			}
			else
			{
				if (source_.size())
				{
					utils::Sprintf(full_desc_, "EXCEPTION(%d:%s): \"%s\" in %s", type_, title_.c_str(), description_.c_str(), source_.c_str());
				}
				else
				{
					utils::Sprintf(full_desc_, "EXCEPTION(%d:%s): \"%s\".", type_, title_.c_str(), description_.c_str());
				}

			}
		}

		return full_desc_;
	}

    int Exception::GetType(void) const throw()
	{
		return type_;
	}

    const string &Exception::GetSource() const 
	{ 
		return source_;
	}

    const string &Exception::GetFile() const 
	{ 
		return file_; 
	}

    long Exception::GetLine() const 
	{ 
		return line_; 
	}

	const string &Exception::GetDescription(void) const 
	{ 
		return description_; 
	}

	const char* Exception::what() const throw() 
	{ 
		return GetFullDescription().c_str(); 
	}
}