/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <build.hpp>
#include <string>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>

namespace Xmaxapp
{
	namespace options = boost::program_options;

	namespace asio = boost::asio;

	using options_desc = options::options_description;
	using vars_map = options::variables_map;
	using app_service = asio::io_service;
	using string = std::string;


	class appbase;
	class plugin;
}