/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <apps.hpp>

namespace Xmaxapp
{
	class plugin_face;
	class plugin_factory;

	using plugin_factory_function = plugin_face* (appbase*);

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

	class plugin_face : public plugin
	{
		//GENERATED_PLUGIN(plugin_face, plugin);
	public:

		virtual state get_state() const override
		{
			return plugin_state;
		}		
		virtual appbase* get_app() const override
		{
			return _plugin_owner;
		}

		virtual void set_options(options_desc& cli, options_desc& cfg)
		{

		}
		virtual void initialize(const vars_map& options)
		{
			plugin_state = plugin::state::initialized;
		}

		virtual void startup()
		{
			plugin_state = plugin::state::startuped;
		}
		virtual void run()
		{
			plugin_state = plugin::state::running;
		}
		virtual void shutdown()
		{
			plugin_state = plugin::state::stopped;
		}
	protected:
		plugin_face()
			: plugin_state(plugin::state::unknown)
			, _plugin_owner(nullptr)
		{

		}
		template<typename TPlugin>
		static TPlugin* create_plugin_impl(appbase* _owner)
		{
			TPlugin* _self = new TPlugin();
			_self->set_owner(_owner);
			return _self;
		}

	private:
		void set_owner(appbase* _owner)
		{
			_plugin_owner = _owner;
		}
		plugin::state plugin_state;
		appbase* _plugin_owner;
	};



#define GENERATED_PLUGIN(plugin_self, super_class) \
	public:\
		typedef super_class super;\
		static const Xmaxapp::string& plugin_name()\
		{	\
			static const Xmaxapp::string _plugin_name(#plugin_self);\
			return _plugin_name;\
		}	\
		static Xmaxapp::plugin_factory get_plugin_factory()\
		{\
			return Xmaxapp::plugin_factory(plugin_name(), create_plugin);\
		}\
		virtual const Xmaxapp::string& get_name() const override\
		{\
			return plugin_name();\
		}\
	private:\
		static Xmaxapp::plugin_face* create_plugin(Xmaxapp::appbase* _owner)\
		{\
			return create_plugin_impl<plugin_self>(_owner);\
		}


}