/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <app_types.hpp>

namespace Xmaxapp
{
	class plugin
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

		virtual ~plugin() {}
		virtual state get_state() const = 0;
		virtual const string& get_name() const = 0;

		virtual appbase* get_app() const = 0;
	};

	class appbase
	{
	public:
		virtual ~appbase() {}

		virtual app_service* get_service() const = 0;
	};
}