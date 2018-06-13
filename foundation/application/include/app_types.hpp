/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <pro/types/build.hpp>
#include <string>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>

namespace xmaxapp
{
	namespace options = boost::program_options;

	namespace asio = boost::asio;

	using string = std::string;

	using OptionsDesc = options::options_description;
	using VarsMap = options::variables_map;
	using AppService = asio::io_service;



	class ApplicationBase;
	class Plugin;
}