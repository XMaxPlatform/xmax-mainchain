/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <apps.hpp>
#include <app_types.hpp>

namespace xmaxapp
{
	class PluginFace;
	class PluginFactory;

	using PluginFactoryFunction = PluginFace* (ApplicationBase*);
	using PluginInitOptions = void (OptionsDesc& cli, OptionsDesc& cfg);

	class PluginFactory
	{
	public:

		PluginFace* CreatePlugin(ApplicationBase* owner)
		{
			if (create_function_)
			{
				return create_function_(owner);
			}
			return nullptr;
		}

		const string& GetName() const
		{
			return plugin_name_;
		}

		void InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
		{
			if (init_options_)
			{
				init_options_(cli, cfg);
			}
		}

	private:
		PluginFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2)
			: plugin_name_(_name)
			, create_function_(_function)
			, init_options_(_function2)
		{

		}
		string plugin_name_;
		std::function<PluginFactoryFunction> create_function_;
		std::function<PluginInitOptions> init_options_;

	public:

		static bool IsRegist(const string& name);
		static PluginFactory* GetPluginFactory(const string& name);
		static PluginFace* NewPlugin(const string& name, ApplicationBase* owner);
		static bool RegistFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2);
	protected:
		static std::map<string, std::unique_ptr<PluginFactory>> sPluginFactorys;
	};

	class PluginFace : public Plugin
	{
		//GENERATED_PLUGIN(plugin_face, plugin);
	public:
		virtual const string& GetName() const = 0;

		virtual State GetState() const override
		{
			return plugin_state_;
		}		
		virtual ApplicationBase* GetApp() const override
		{
			return plugin_owner_;
		}

		virtual void Initialize(const VarsMap& options)
		{
			plugin_state_ = Plugin::State::initialized;
		}

		virtual void Startup()
		{
			plugin_state_ = Plugin::State::startuped;
		}
		virtual void Shutdown()
		{
			plugin_state_ = Plugin::State::stopped;
		}
	protected:
		PluginFace()
			: plugin_state_(Plugin::State::unknown)
			, plugin_owner_(nullptr)
		{

		}
		template<typename TPlugin>
		static TPlugin* CreatePluginImpl(ApplicationBase* _owner)
		{
			TPlugin* _self = new TPlugin();
			_self->SetOwner(_owner);
			return _self;
		}

	private:
		void SetOwner(ApplicationBase* _owner)
		{
			plugin_owner_ = _owner;
		}
		Plugin::State plugin_state_;
		ApplicationBase* plugin_owner_;
	};

}

#define GENERATED_PLUGIN(plugin_self, super_class, init_opt) \
	public:\
		typedef super_class Super;\
		static const xmaxapp::string& PluginName()\
		{	\
			static const xmaxapp::string _plugin_name(#plugin_self);\
			return _plugin_name;\
		}	\
		static bool RegistSelf()\
		{\
			return xmaxapp::PluginFactory::RegistFactory(PluginName(), createPlugin, init_opt);\
		}\
		virtual const xmaxapp::string& GetName() const override\
		{\
			return PluginName();\
		}\
	private:\
		static xmaxapp::PluginFace* createPlugin(xmaxapp::ApplicationBase* _owner)\
		{\
			return CreatePluginImpl<plugin_self>(_owner);\
		}

//#define ALWAYS_REGIST_PLUGIN(plugin_self) \
//static bool b##plugin_self##_xxx = plugin_self::RegistSelf();
