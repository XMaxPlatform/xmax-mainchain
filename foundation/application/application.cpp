/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <log.hpp>
#include "application.hpp"

namespace Xmaxapp
{
	using boost::program_options::variables_map;

	application::application()
	{
		service_face = std::make_unique<app_service>();
	}

	application::~application()
	{

	}

	app_service* application::get_service() const
	{
		return service_face.get();
	}

	void application::initialize(int argc, char** argv)
	{
		// create plugin objects.
		for (auto item : plugin_factorys)
		{
			if (plugin_face* plugin = item.second.factory_function(this))
			{
				pluginmap[item.first].reset(plugin);
				initialized_plugins.push_back(plugin);
			}
		}

		for (auto item : initialized_plugins)
		{
			options_desc plugin_cli_opts("Command Line for " + item->get_name());
			options_desc plugin_cfg_opts("Config for " + item->get_name());
			item->set_options(plugin_cli_opts, plugin_cfg_opts);
			if (plugin_cfg_opts.options().size()) {
				app_options.add(plugin_cfg_opts);
				cfg_options.add(plugin_cfg_opts);
			}
			if (plugin_cli_opts.options().size())
				app_options.add(plugin_cli_opts);
		}

		// parse options.
		variables_map options;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, app_options), options);

		// 
		for (auto item : initialized_plugins)
		{			
			ilog("Plugin '%s' initialize. ", item->get_name().c_str());
			item->initialize(options);
		}
	}

	void application::startup()
	{
		for (auto item : initialized_plugins)
		{
			if (item)
			{
				ilog("Plugin '%s' startup. ", item->get_name().c_str());
				item->startup();
				startup_plugins.push_back(item);
			}
		}
	}

	void application::shutdown()
	{
		for (auto item : initialized_plugins)
		{
			if (item)
			{
				ilog("Plugin '%s' shutdown. ", item->get_name().c_str());
				item->shutdown();
			}
		}
	}

	void application::quit() 
	{
		service_face->stop();
	}

	void application::loop() 
	{
		std::shared_ptr<boost::asio::signal_set> sigint_set(new boost::asio::signal_set(*service_face, SIGINT));
		sigint_set->async_wait([sigint_set, this](const boost::system::error_code& err, int num) 
		{
			quit();
			sigint_set->cancel();
		});

		std::shared_ptr<boost::asio::signal_set> sigterm_set(new boost::asio::signal_set(*service_face, SIGTERM));
		sigterm_set->async_wait([sigterm_set, this](const boost::system::error_code& err, int num) 
		{
			quit();
			sigterm_set->cancel();
		});

		service_face->run();

		shutdown(); /// perform synchronous shutdown
	}

	void application::regist_plugin(plugin_factory&& _factory)
	{
		plugin_factorys[_factory.plugin_name] = _factory;
	}

bool application::check_regist(const string& name) const
	{
		auto itr = plugin_factorys.find(name);
		if (itr == plugin_factorys.end())
		{
			return true;
		}
		iwarnning("plugin '%s' had already registed.", name.c_str());
		return false;
	}
}