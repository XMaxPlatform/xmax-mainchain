/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <plugin.hpp>

namespace Xmax
{
	using plugin_factory_function = iplugin* (void);

	class plugin_factory
	{
	public:
		string plugin_name;
		std::function<plugin_factory_function> factory_function;

		plugin_factory(const string& _name, const std::function<plugin_factory_function>& _function)
			: plugin_name(_name)
			, factory_function(_function)
		{

		}
		plugin_factory()
		{

		}
	};

	class plugin_face : public iplugin
	{
		//GENERATED_PLUGIN(plugin_face, plugin);
	public:

		virtual state get_state() const override
		{
			return plugin_state;
		}		

		virtual void set_program_options(options_description& cli, options_description& cfg) override
		{

		}
		virtual void initialize(const variables_map& options) override
		{
			plugin_state = iplugin::state::initialized;
		}

		virtual void startup() override
		{
			plugin_state = iplugin::state::startuped;
		}
		virtual void run() override
		{
			plugin_state = iplugin::state::running;
		}
		virtual void shutdown() override
		{
			plugin_state = iplugin::state::stopped;
		}
	protected:
		plugin_face()
			:plugin_state(iplugin::state::unknown)
		{

		}
	private:
		iplugin::state plugin_state;
	};



#define GENERATED_PLUGIN(plugin_self, super_class) \
	public:\
		typedef super_class super;\
		static const Xmax::Generictypes::string& plugin_name()\
		{	\
			static const Xmax::Generictypes::string _plugin_name(#plugin_self);\
			return _plugin_name;\
		}	\
		static plugin_factory get_plugin_factory()\
		{\
			return plugin_factory(plugin_name(), create_plugin);\
		}\
		virtual const Xmax::Generictypes::string& get_name() const override\
		{\
			return plugin_name();\
		}\
	private:\
		static plugin_self* create_plugin()\
		{\
			return new plugin_self(); \
		}


}