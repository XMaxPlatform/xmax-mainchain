/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <apps.hpp>
#include <app_types.hpp>
#include <unordered_map>

namespace xmaxapp
{
	class PluginFace;
	class PluginFactory;

	using PluginFactoryFunction = PluginFace* (ApplicationBase*);
	using PluginInitOptions = void (OptionsDesc& cli, OptionsDesc& cfg);
	using DependentPluginVistior = std::function<void(const std::string& plugin_name)>;

	class PluginFactory
	{
	public:
	    /**
		 *create a plugin
		 * @param[in] ApplicationBase
		 */
		PluginFace* CreatePlugin(ApplicationBase* owner);
		/**
		 *name of plugin
		 */
		const string& GetName() const;
		/**
		* initialize options
		* @param[in] OptionsDesc option desc
		* @param[in] OptionsDesc option cfg
		*/
		void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);

		/**
		* Visit dependent plugin recursively
		* @param[in] DependentPluginVistior visitor function		
		*/
		void VisitDependentPluginsRecursively(DependentPluginVistior visitor);

	private:
		/**
		* constructor 
		* @param[in] string name of plugin
		* @param[in] std::function factory method
		* @param[in] std::function initoptions
		*/		
		PluginFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2, std::optional<const std::vector<std::string>> op_dependent_plugins);

		string plugin_name_;
		std::function<PluginFactoryFunction> create_function_;
		std::function<PluginInitOptions> init_options_;
		std::vector<std::string> dependent_plugins_;

	public:
		/**
		* return if this factory is created
		* @param[in] string name of plugin
		*/
		static bool IsRegist(const string& name);
		/**
		* get factory
		* @param[in] string name of plugin
		*/
		static PluginFactory* GetPluginFactory(const string& name);
		/**
		* create a plugin
		* @param[in] string				name of plugin
		* @param[in] ApplicationBase  
		*/
		static PluginFace* NewPlugin(const string& name, ApplicationBase* owner);
		/**
		* register a factory
		* @param[in] string				name of plugin
		* @param[in] std::function		factory method
		* @param[in] std::function		init options
		*/
		static bool RegistFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2, 
			std::vector<string> dependent_plugins);
	protected:
		static std::unordered_map<string, std::unique_ptr<PluginFactory>> sPluginFactorys;
	};

	class PluginFace : public Plugin
	{
		//GENERATED_PLUGIN(plugin_face, plugin);
	public:
		/**
		* return the state of plugin
		*/
		virtual State GetState() const override;

		virtual ApplicationBase* GetApp() const override;
		/**
		* set state of the plugin
		* @param[in] options::variables_map		param map
		*/
		virtual void Initialize(const VarsMap& options);
		/**
		* start a plugin
		*/
		virtual void Startup();
		/**
		* stutdown a plugin
		*/
		virtual void Shutdown();
		/**
		* return the name of plugin
		*/
		virtual const string& GetName() const = 0;

		friend class PluginFactory;

	protected:
		PluginFace();

		virtual void OnCreated();

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

/**
* this macro help you generate code of a concrete plugin
*/
#define GENERATED_PLUGIN(plugin_self, super_class, init_opt, ...) \
	public:\
		typedef super_class Super;\
		static const xmaxapp::string& PluginName()\
		{	\
			static const xmaxapp::string _plugin_name(#plugin_self);\
			return _plugin_name;\
		}	\
		static bool RegistSelf()\
		{\
			return xmaxapp::PluginFactory::RegistFactory(PluginName(), createPlugin, init_opt, {#__VA_ARGS__});\
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
