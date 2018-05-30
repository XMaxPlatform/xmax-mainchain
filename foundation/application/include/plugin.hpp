/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <boost/program_options.hpp>

#include <generictypes.hpp>

namespace Xmax
{
	using boost::program_options::options_description;
	using boost::program_options::variables_map;
	using Xmax::Generictypes::string;

	class iplugin
	{
	public:
		enum class state
		{
			unknown = 0,
			initialized, 
			startuped,
			running, 
			stopped 
		};

		virtual ~iplugin() {}
		virtual state get_state() const = 0;
		virtual const string& get_name() const = 0;
		virtual void set_program_options(options_description& cli, options_description& cfg) = 0;

		virtual void initialize(const variables_map& options) = 0;
		virtual void startup() = 0;
		virtual void run() = 0;
		virtual void shutdown() = 0;
	};
}