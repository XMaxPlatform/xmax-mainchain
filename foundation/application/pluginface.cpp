/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include "pluginface.hpp"

namespace xmaxapp
{
	std::unordered_map<string, std::unique_ptr<PluginFactory>> PluginFactory::sPluginFactorys;


	PluginFactory::PluginFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2
	, std::optional<const std::vector<std::string>> op_dependent_plugins)
		: plugin_name_(_name)
		, create_function_(_function)
		, init_options_(_function2)		
	{
		if (op_dependent_plugins) {
			dependent_plugins_ = op_dependent_plugins.value();
		}
	}

	PluginFace* PluginFactory::CreatePlugin(ApplicationBase* owner)
	{
		if (create_function_)
		{
			auto plugin = create_function_(owner);
			plugin->OnCreated();
			return plugin;
		}
		return nullptr;
	}

	const string& PluginFactory::GetName() const
	{
		return plugin_name_;
	}

	void PluginFactory::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		if (init_options_)
		{
			init_options_(cli, cfg);
		}
	}


	//--------------------------------------------------
	void PluginFactory::VisitDependentPluginsRecursively(DependentPluginVistior visitor)
	{
		for (auto& name : dependent_plugins_)
		{
			auto factory = GetPluginFactory(name);
			if (factory) {
				factory->VisitDependentPluginsRecursively(visitor);
			}
			visitor(name);
		}

	}

	bool PluginFactory::RegistFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2,
		std::vector<string> dependent_plugins)
	{
		if (sPluginFactorys.find(_name) == sPluginFactorys.end())
		{
			sPluginFactorys[_name].reset(new PluginFactory(_name, _function, _function2, std::move(dependent_plugins)));
			return true;
		}
		else
		{
			Warnf( "plugin '${name}' had already regist.", ("name", _name) );
			return false;
		}
	}

	bool PluginFactory::IsRegist(const string& name)
	{
		auto itr = sPluginFactorys.find(name);
		if (itr != sPluginFactorys.end())
		{
			return true;
		}
		return false;
	}

	PluginFactory* PluginFactory::GetPluginFactory(const string& name)
	{
		auto itr = sPluginFactorys.find(name);
		if (itr != sPluginFactorys.end())
		{
			return itr->second.get();
		}
		return nullptr;
	}

	PluginFace* PluginFactory::NewPlugin(const string& name, ApplicationBase* owner)
	{
		if (nullptr == owner)
		{
			return nullptr;
		}
		if (auto itr = GetPluginFactory(name))
		{
			return itr->CreatePlugin(owner);
		}
		return nullptr;
	}


	PluginFace::PluginFace()
		: plugin_state_(Plugin::State::unknown)
		, plugin_owner_(nullptr)
	{

	}

	Plugin::State PluginFace::GetState() const
	{
		return plugin_state_;
	}
	ApplicationBase* PluginFace::GetApp() const
	{
		return plugin_owner_;
	}

	void PluginFace::Initialize(const VarsMap& options)
	{
		plugin_state_ = Plugin::State::initialized;
	}

	void PluginFace::Startup()
	{
		plugin_state_ = Plugin::State::startuped;
	}
	void PluginFace::Shutdown()
	{
		plugin_state_ = Plugin::State::stopped;
	}

	void PluginFace::OnCreated()
	{

	}

}
