/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <boost/program_options.hpp>

#include <generictypes.hpp>
namespace Xmaxapp
{
	namespace options = boost::program_options;

	using options_desc = options::options_description;
	using vars_map = options::variables_map;
	using string = Xmax::Generictypes::string;
}