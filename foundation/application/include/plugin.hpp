/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <app_types.hpp>

namespace Xmaxapp
{
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
		virtual void set_options(options_desc& cli, options_desc& cfg) = 0;

		virtual void initialize(const vars_map& options) = 0;
		virtual void startup() = 0;
		virtual void run() = 0;
		virtual void shutdown() = 0;
	};
}