/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include "pluginface.hpp"

namespace Xmaxapp
{
	std::map<string, std::unique_ptr<plugin_factory>> plugin_factory::plugin_factorys;

	bool plugin_factory::regist_factory(const string& _name, const std::function<plugin_factory_function>& _function, const std::function<plugin_init_options>& _function2)
	{
		if (plugin_factorys.find(_name) == plugin_factorys.end())
		{
			plugin_factorys[_name].reset(new plugin_factory(_name, _function, _function2));
			return true;
		}
		else
		{
			iwarnning("plugin '%s' had already regist.", _name);
			return false;
		}
	}

	bool plugin_factory::is_regist(const string& name)
	{
		auto itr = plugin_factorys.find(name);
		if (itr != plugin_factorys.end())
		{
			return true;
		}
		return false;
	}

	plugin_factory* plugin_factory::get_plugin_factory(const string& name)
	{
		auto itr = plugin_factorys.find(name);
		if (itr != plugin_factorys.end())
		{
			return itr->second.get();
		}
		return nullptr;
	}

	plugin_face* plugin_factory::new_plugin(const string& name, appbase* owner)
	{
		if (nullptr == owner)
		{
			return nullptr;
		}
		if (auto itr = get_plugin_factory(name))
		{
			return itr->create_plugin(owner);
		}
		return nullptr;
	}
}
