/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <app_types.hpp>
#include <pro/io/file_system.hpp>

namespace xmaxapp
{

	/**
	* Base class of all plugins
	*/
	class Plugin
	{
	public:
		/**
		*  State of Plugin
		* 
		*/
		enum class State
		{
			unknown = 0,
			initialized, 
			startuped,
			stopped 
		};

		virtual ~Plugin() {}
		/**
		* return the state of Plugin
		*/
		virtual State GetState() const = 0;
		/**
		*  return the name of plugin
		*/
		virtual const string& GetName() const = 0;
		/**
		* return ApplicationBase
		*/
		virtual ApplicationBase* GetApp() const = 0;
	};

	/**
	*  base class of ApplicationBase
	*  pure virtual class
	*/
	class ApplicationBase
	{
	public:
		virtual ~ApplicationBase() {}
		/**
		* return AppService
		*/
		virtual AppService* GetService() const = 0;
		/**
		* return data dir
		*/
		virtual fs::path	GetDataDir() const = 0;
	};
}