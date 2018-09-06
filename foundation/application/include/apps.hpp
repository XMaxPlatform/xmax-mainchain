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
		*/
		enum class State
		{
			kUnkown = 0,	/*!< unknown state. */  
			kInitialized,	/*!< initialized state. */  
			kStartuped,		/*!< startuped state. */  
			kStopped			/*!< stopped state. */  
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
		* @return AppService
		*/
		virtual AppService* GetService() const = 0;
		/**
		* @return data dir
		*/
		virtual fs::path GetDataDir() const = 0;

		/**
		* find plugin by name
		* @return plugin
		*/
		virtual Plugin* FindPlugin(const string& name) const = 0;
		/**
		* get plugin by name
		* @return plugin
		*/
		virtual Plugin& GetPlugin(const string& name) const = 0;


		template<typename PluginType>
		PluginType* FindPlugin() const
		{
			return dynamic_cast<PluginType*>(FindPlugin(PluginType::PluginName()));
		}
		template<typename PluginType>
		PluginType& GetPlugin() const
		{
			auto ptr = FindPlugin<PluginType>();
			return *ptr;
		}
	};
}