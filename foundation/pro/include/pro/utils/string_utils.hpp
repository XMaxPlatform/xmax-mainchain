/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <pro/types/generictypes.hpp>
#include <pro/utils/string_misc.hpp>

namespace pro
{
	class AnyObject;

	namespace utils
	{
		/**
		* format string.
		* @param[out] formated string buff.
		* @param[in] format of string.
		* @param[in] ... args.
		*/
		void Sprintf(string& buff, const char* format, ...);

		/**
		* format string.
		* @param[in] format of string.
		* @param[in] ... args.
		* @return format string.
		*/
		string Sprintf(const char* format, ...);


		/**
		* format string.
		* @param[out] formated string buff.
		* @param[in] format of string.
		* @param[in] args args.
		*/
		void StringFormat(string& buff, const string& format, const AnyObject& args);
		/**
		* format string.
		* @param[in] format of string.
		* @param[in] args args
		* @return format string.
		*/
		string StringFormat(const string& format, const AnyObject& args);

		template <typename T>
		static void ToString(string& str, const T& v)
		{
			str = std::to_string(v);
		}

		static void ToString(string& str, const string& v)
		{
			str = v;
		}

		template <typename T>
		static string ToString(const T& v)
		{
			string str;
			ToString(str, v);
			return str;
		}
	}


#define PACK_ARGS(...) ((pro::ArgsPack()__VA_ARGS__).GetArgs())
#define STRING_FORMAT(_fmt, ...) pro::utils::StringFormat(_fmt, PACK_ARGS(__VA_ARGS__))
}


