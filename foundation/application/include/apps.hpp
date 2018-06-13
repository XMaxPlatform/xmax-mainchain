/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <app_types.hpp>

namespace xmaxapp
{
	class Plugin
	{
	public:
		enum class State
		{
			unknown = 0,
			initialized, 
			startuped,
			stopped 
		};

		virtual ~Plugin() {}
		virtual State GetState() const = 0;
		virtual const string& GetName() const = 0;

		virtual ApplicationBase* GetApp() const = 0;
	};

	class ApplicationBase
	{
	public:
		virtual ~ApplicationBase() {}

		virtual AppService* GetService() const = 0;
	};
}