/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once

#include <apps.hpp>
#include <app_types.hpp>
#include <unordered_map>
#include <boost/preprocessor/variadic/to_list.hpp>
#include <boost/preprocessor/list/for_each.hpp>

namespace xmaxapp
{
	class PluginFace;
	class PluginFactory;

	using PluginFactoryFunction = PluginFace* (ApplicationBase*);
	using PluginInitOptions = void (OptionsDesc& cli, OptionsDesc& cfg);
	using DependentPluginVistior = std::function<void(const std::string& plugin_name)>;


	/**
	*  plugin factory.
	*/
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
		* @param[in] OptionsDesc program options which passed by program command line arguments
		* @param[in] OptionsDesc configuration options which set in configuration file and also supported passing by program command line arguments
		*/
		void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);

		/**
		* Visit dependent plugin recursively
		* @param[in] DependentPluginVistior visitor function		
		*/
		void VisitDependentPluginsRecursively(DependentPluginVistior visitor);

		/**
		* register a plugin's dependent plugins.
		* @param[in] string				dependent plugin vector
		*/
		bool RegistPluginDependencies(std::vector<string> dependent_plugins);

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
		static bool RegistFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2);
		

	private:
		/**
		* constructor 
		* @param[in] string name of plugin
		* @param[in] std::function factory method
		* @param[in] std::function initoptions
		*/		
		PluginFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2);


	protected:
		static std::unordered_map<string, std::unique_ptr<PluginFactory>> sPluginFactorys;

	private:
		string plugin_name_;
		std::function<PluginFactoryFunction> create_function_;
		std::function<PluginInitOptions> init_options_;
		std::vector<std::string> dependent_plugins_;	

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

		static void RegistPluginDependencies() {}

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

#define MACRO_TO_QUOTE(r, data, elem) #elem, 


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
			bool ret = xmaxapp::PluginFactory::RegistFactory(PluginName(), createPlugin, init_opt);\
			RegistPluginDependencies();\
			return ret;\
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

#define PLUGIN_DEPENDS(...) \
	protected:\
		static void RegistPluginDependencies() { \
			xmaxapp::PluginFactory::GetPluginFactory(PluginName())->RegistPluginDependencies( \
				{ BOOST_PP_LIST_FOR_EACH(MACRO_TO_QUOTE, _, BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__)) }); \
		}

//#define ALWAYS_REGIST_PLUGIN(plugin_self) \
//static bool b##plugin_self##_xxx = plugin_self::RegistSelf();
