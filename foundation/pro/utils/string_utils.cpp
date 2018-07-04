
#include <cstdarg>
#include <sstream>
#include <pro/types/any_object.hpp>
#include <pro/utils/string_utils.hpp>

constexpr size_t sprintf_buff_size = 1024;


static constexpr size_t key_format_len_min = 4;

namespace pro
{
	namespace utils
	{
		void Sprintf(string& buff, const char* format, ...)
		{
			char msg[sprintf_buff_size] = { 0 };
			va_list arg_ptr;
			va_start(arg_ptr, format);
			int nWrittenBytes = vsprintf_s(msg, sprintf_buff_size - 1, format, arg_ptr);
			va_end(arg_ptr);
			buff = msg;
		}

		string Sprintf(const char* format, ...)
		{
			char msg[sprintf_buff_size] = { 0 };
			va_list arg_ptr;
			va_start(arg_ptr, format);
			int nWrittenBytes = vsprintf_s(msg, sprintf_buff_size - 1, format, arg_ptr);
			va_end(arg_ptr);

			return msg;
		}

		string StringFormat(const string& format, const AnyObject& args)
		{
			string buff;
			StringFormat(buff, format, args);
			return buff;
		}

		void StringFormat(string& buff, const string& format, const AnyObject& args)
		{
			// --------- example --------
			//	   sk
			//	0  1  2  3  4  5
			//  a  $  {  x  }  z
			// --------------------------

			if (format.size() < key_format_len_min)
			{
				buff = format;
				return;
			}

			std::stringstream sstr;

			size_t length = format.size();			

			size_t cpybeg = 0;
			size_t cpyend = 0;

			while (true)
			{
				auto seek = format.find('$', cpyend); // sk

				if (seek == string::npos)// search end.
				{
					cpyend = length;
					break;
				}

				if ((length - seek) < key_format_len_min)// search end.
				{
					cpyend = length;
					break; 
				}

				if (format[seek + 1] != '{')// continue search.
				{
					cpyend = seek + 2;
					continue; 
				}

				auto keyeof = format.find('}', seek + 3);
				if (keyeof == string::npos)// search end.
				{
					cpyend = length;
					break;
				}
				// read key
				auto keybeg = seek + 2;
				string key = format.substr(keybeg, keyeof - keybeg);

				const auto& V = args[key];
				if (V.IsValid())
				{
					// push chars before '$'
					sstr.write(&format[cpybeg], seek - cpybeg);
					// push replace string of key.
					sstr << V.ToString();


					cpybeg = keyeof + 1;
					cpyend = cpybeg;
					continue; // continue search.
				}
				else
				{
					cpyend = keyeof + 1;
				}		
			}

			// copy last strings.
			if (cpyend > cpybeg && cpyend <= length)
			{
				sstr.write(&format[cpybeg], cpyend - cpybeg);
			}

			buff = sstr.str();
		}

	}
}