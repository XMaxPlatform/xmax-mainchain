/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <exception>
#include <pro/types/generictypes.hpp>

namespace pro
{
	#define BASE_EXCEPT(desc, src) throw Exception(desc, __FILE__, __LINE__)

	enum ExceptionType : int
	{
		EXT_UNDEF_TYPE = 0,
		EXT_ASSERT_ERROR = 1,
		EXT_SERIALIZE_ERROR = 100,
		EXT_FORMAT_ERROR = 200,
		EXT_MEMORY_ERROR = 300,

		//EXT_FILE_READ_FALSE,
		//EXT_FILE_WRITE_FALSE,
		//EXT_FILE_NOT_FOUND,
		//EXT_ITEM_NOT_FOUND,
		//EXT_SYSTEM_ERROR,
		EXT_CUSTOM_BEGIN = 1000,
	};

	class ExceptionImpl;

	class Exception : public std::exception
	{
	public:
		

		/** Default constructor.
        */

		Exception(const string& _description, const string& _source, const char* _file, long _line);
		Exception(string&& _description, string&& _source, const char* _file, long _line);

		Exception(std::string _description, const string _source);
		Exception(string _description, const char* _file, long _line);

        /** Copy constructor.
        */
        Exception(const Exception& rhs);
		Exception(Exception&& rhs);

		/// Needed for  compatibility with std::exception
		virtual ~Exception() throw();

        /** Assignment operator.
        */
		Exception& operator = (const Exception& rhs);
		Exception& operator = (Exception&& rhs);

        string GetFullDescription() const;

        /** Gets the error code.
        */
        virtual int GetType(void) const throw();

        /** Gets the source function.
        */
        virtual const string &GetSource() const;

        /** Gets source file name.
        */
        virtual const string &GetFile() const;

        /** Gets line number.
        */
        virtual long GetLine() const;

		/** Returns a string with only the 'description' field of this exception. Use 
			GetFullDescriptionto get a full description of the error including line number,
			error number and what function threw the exception.
        */
		virtual const string &GetDescription(void) const;

		/// Override std::exception::what
		const char* what() const throw();

	protected:

		/** Advanced constructor.
        */
        Exception(int _type, string&& _description, string&& _source, const char* _tile, const char* _file, long _line);
		Exception(int _type, string&&_description, const char* _tile, const char* _file, long _line);

		Exception(int _type, const string& _description, const string& _source, const char* _tile, const char* _file, long _line);
		Exception(int _type, const string& _description, const char* _tile, const char* _file, long _line);
						
		//Shared implementation ptr
		std::shared_ptr<ExceptionImpl> impl_;
	};
}
