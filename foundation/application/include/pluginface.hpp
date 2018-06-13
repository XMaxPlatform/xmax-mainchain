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
	using plugin_init_options = void (options_desc& cli, options_desc& cfg);

	class plugin_factory
	{
	public:

		plugin_face* create_plugin(appbase* owner)
		{
			if (_create_function)
			{
				return _create_function(owner);
			}
			return nullptr;
		}

		const string& get_name() const
		{
			return plugin_name;
		}

		void init_options(options_desc& cli, options_desc& cfg)
		{
			if (_init_options)
			{
				_init_options(cli, cfg);
			}
		}

	private:
		plugin_factory(const string& _name, const std::function<plugin_factory_function>& _function, const std::function<plugin_init_options>& _function2)
			: plugin_name(_name)
			, _create_function(_function)
			, _init_options(_function2)
		{

		}
		string plugin_name;
		std::function<plugin_factory_function> _create_function;
		std::function<plugin_init_options> _init_options;

	public:

		static bool is_regist(const string& name);
		static plugin_factory* get_plugin_factory(const string& name);
		static plugin_face* new_plugin(const string& name, appbase* owner);
		static bool regist_factory(const string& _name, const std::function<plugin_factory_function>& _function, const std::function<plugin_init_options>& _function2);
	protected:
		static std::map<string, std::unique_ptr<plugin_factory>> plugin_factorys;
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

}

#define GENERATED_PLUGIN(plugin_self, super_class, init_opt) \
	public:\
		typedef super_class super;\
		static const Xmaxapp::string& plugin_name()\
		{	\
			static const Xmaxapp::string _plugin_name(#plugin_self);\
			return _plugin_name;\
		}	\
		static bool regist_self()\
		{\
			return Xmaxapp::plugin_factory::regist_factory(plugin_name(), create_plugin, init_opt);\
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

//#define ALWAYS_REGIST_PLUGIN(plugin_self) \
//static bool b##plugin_self##_xxx = plugin_self::regist_self();
