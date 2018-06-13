/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include "pluginface.hpp"

namespace xmaxapp
{
	std::map<string, std::unique_ptr<PluginFactory>> PluginFactory::sPluginFactorys;

	bool PluginFactory::RegistFactory(const string& _name, const std::function<PluginFactoryFunction>& _function, const std::function<PluginInitOptions>& _function2)
	{
		if (sPluginFactorys.find(_name) == sPluginFactorys.end())
		{
			sPluginFactorys[_name].reset(new PluginFactory(_name, _function, _function2));
			return true;
		}
		else
		{
			iwarnning("plugin '%s' had already regist.", _name);
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
}
