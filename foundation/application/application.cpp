/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
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

	void application::plugin_to_init(const string& plugin_name)
	{
		if (pluginmap.find(plugin_name) == pluginmap.end())
		{
			if (plugin_factory::is_regist(plugin_name))
			{
				pluginmap[plugin_name].reset();
			}
			else
			{
				iwarnning("Unknown plugin:'%s'.", plugin_name.c_str());
			}
		}
		else
		{
			iwarnning("Plugin:'%s' had in the init list.", plugin_name.c_str());
		}

	}

	void application::initialize(int argc, char** argv)
	{

		for (auto& item : pluginmap)
		{
			if (plugin_factory* factory = plugin_factory::get_plugin_factory(item.first))
			{
				options_desc plugin_cmd_opts("Command Line for " + factory->get_name());
				options_desc plugin_cfg_opts("Config for " + factory->get_name());
				factory->init_options(plugin_cmd_opts, plugin_cfg_opts);

				if (plugin_cfg_opts.options().size()) {
					app_options.add(plugin_cfg_opts);
					cfg_options.add(plugin_cfg_opts);
				}

				if (plugin_cmd_opts.options().size())
					app_options.add(plugin_cmd_opts);
			}
		}


		options_desc app_cfg_opts("Application Config Options");
		options_desc app_cmd_opts("Application Command Line Options");

		cfg_options.add(app_cfg_opts);
		app_options.add(app_cfg_opts);
		app_options.add(app_cmd_opts);


		app_cfg_opts.add_options()
			("plugin", options::value< std::vector<string> >()->composing(), "Plugin(s) to startup.");

		// create plugin objects.

		for (auto& item : pluginmap)
		{
			if (plugin_face* plugin = plugin_factory::new_plugin(item.first, this))
			{
				pluginmap[item.first].reset(plugin);
				initialized_plugins.push_back(plugin);
			}
		}

		// parse options.
		variables_map option_vars;

		//options::store(options::parse_config_file<char>(config_file_name.make_preferred().string().c_str(),
		//	cfg_options, true), option_vars);

		options::store(options::parse_command_line(argc, argv, app_options), option_vars);

		// 
		for (auto item : initialized_plugins)
		{

			item->initialize(option_vars);
			ilog("Plugin '%s' initialize. ", item->get_name().c_str());
		}
	}

	void application::startup()
	{
		for (auto item : initialized_plugins)
		{
			if (item)
			{

				item->startup();
				startup_plugins.push_back(item);
				ilog("Plugin '%s' startup. ", item->get_name().c_str());
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

}