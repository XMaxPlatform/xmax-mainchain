/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include "application.hpp"

namespace xmaxapp
{
	using boost::program_options::variables_map;

	Application::Application()
	{
		service_face = std::make_unique<AppService>();
	}

	Application::~Application()
	{

	}

	AppService* Application::GetService() const
	{
		return service_face.get();
	}

	void Application::PluginToInit(const string& plugin_name)
	{
		if (pluginmap.find(plugin_name) == pluginmap.end())
		{
			if (PluginFactory::IsRegist(plugin_name))
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

	void Application::Initialize(int argc, char** argv)
	{

		for (auto& item : pluginmap)
		{
			if (PluginFactory* factory = PluginFactory::GetPluginFactory(item.first))
			{
				OptionsDesc plugin_cmd_opts("Command Line for " + factory->GetName());
				OptionsDesc plugin_cfg_opts("Config for " + factory->GetName());
				factory->InitOptions(plugin_cmd_opts, plugin_cfg_opts);

				if (plugin_cfg_opts.options().size()) {
					app_options.add(plugin_cfg_opts);
					cfg_options.add(plugin_cfg_opts);
				}

				if (plugin_cmd_opts.options().size())
					app_options.add(plugin_cmd_opts);
			}
		}


		OptionsDesc app_cfg_opts("Application Config Options");
		OptionsDesc app_cmd_opts("Application Command Line Options");

		cfg_options.add(app_cfg_opts);
		app_options.add(app_cfg_opts);
		app_options.add(app_cmd_opts);


		app_cfg_opts.add_options()
			("plugin", options::value< std::vector<string> >()->composing(), "Plugin(s) to startup.");

		// create plugin objects.

		for (auto& item : pluginmap)
		{
			if (PluginFace* plugin = PluginFactory::NewPlugin(item.first, this))
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

			item->Initialize(option_vars);
			ilog("Plugin '%s' initialize. ", item->GetName().c_str());
		}
	}

	void Application::Startup()
	{
		for (auto item : initialized_plugins)
		{
			if (item)
			{

				item->Startup();
				startup_plugins.push_back(item);
				ilog("Plugin '%s' startup. ", item->GetName().c_str());
			}
		}
	}

	void Application::Shutdown()
	{
		for (auto item : initialized_plugins)
		{
			if (item)
			{
				ilog("Plugin '%s' shutdown. ", item->GetName().c_str());
				item->Shutdown();
			}
		}
	}

	void Application::Quit()
	{
		service_face->stop();
	}

	void Application::Loop()
	{
		std::shared_ptr<boost::asio::signal_set> sigint_set(new boost::asio::signal_set(*service_face, SIGINT));
		sigint_set->async_wait([sigint_set, this](const boost::system::error_code& err, int num)
		{
			Quit();
			sigint_set->cancel();
		});

		std::shared_ptr<boost::asio::signal_set> sigterm_set(new boost::asio::signal_set(*service_face, SIGTERM));
		sigterm_set->async_wait([sigterm_set, this](const boost::system::error_code& err, int num)
		{
			Quit();
			sigterm_set->cancel();
		});

		service_face->run();

		Shutdown(); /// perform synchronous shutdown
	}

}