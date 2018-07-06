/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <pro/types/build.hpp>
#include <string>
#include <pro/io/file_system.hpp>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>

namespace xmaxapp
{
	namespace options = boost::program_options;

	namespace asio = boost::asio;

	using string = std::string;

	namespace fs = pro::fs;

	using OptionsDesc = options::options_description;
	using VarsMap = options::variables_map;
	using AppService = asio::io_service;



	class ApplicationBase;
	class Plugin;
}