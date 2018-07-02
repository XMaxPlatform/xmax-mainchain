/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include <boost/program_options.hpp>
#include "application.hpp"

namespace bpo = boost::program_options;

namespace xmaxapp
{
	using boost::program_options::variables_map;

	Application::Application()
	{
		service_face_ = std::make_unique<AppService>();
	}

	Application::~Application()
	{

	}

	AppService* Application::GetService() const
	{
		return service_face_.get();
	}

	void Application::PluginToInit(const string& plugin_name)
	{
		if (pluginmap_.find(plugin_name) == pluginmap_.end())
		{
			if (PluginFactory::IsRegist(plugin_name))
			{
				pluginmap_[plugin_name].reset();
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

	//--------------------------------------------------
	void Application::SetupApplicationOptions()
	{
		for (auto& item : pluginmap_)
		{
			if (PluginFactory* factory = PluginFactory::GetPluginFactory(item.first))
			{
				OptionsDesc plugin_cmd_opts("Command Line for " + factory->GetName());
				OptionsDesc plugin_cfg_opts("Config for " + factory->GetName());
				factory->InitOptions(plugin_cmd_opts, plugin_cfg_opts);

				if (plugin_cfg_opts.options().size()) {
					app_options_.add(plugin_cfg_opts);
					cfg_options_.add(plugin_cfg_opts);
				}

				if (plugin_cmd_opts.options().size())
					app_options_.add(plugin_cmd_opts);
			}
		}


		OptionsDesc app_cfg_opts("Application Config Options");
		OptionsDesc app_cmd_opts("Application Command Line Options");

		cfg_options_.add(app_cfg_opts);
		app_options_.add(app_cfg_opts);
		app_options_.add(app_cmd_opts);

		app_cfg_opts.add_options()
			("plugin", options::value< std::vector<string> >()->composing(), "Plugin(s) to startup.");
	}


	void Application::Initialize(int argc, char** argv)
	{
		// create plugin objects.
		for (auto& item : pluginmap_)
		{
			if (PluginFace* plugin = PluginFactory::NewPlugin(item.first, this))
			{
				pluginmap_[item.first].reset(plugin);
				initialized_plugins_.push_back(plugin);
			}
		}

		SetupApplicationOptions();


		LoadCfgOptions();
						

		// parse options.
		variables_map option_vars;

		//options::store(options::parse_config_file<char>(config_file_name.make_preferred().string().c_str(),
		//	cfg_options, true), option_vars);

		options::store(options::parse_command_line(argc, argv, app_options_), option_vars);

		// 
		for (auto item : initialized_plugins_)
		{

			item->Initialize(option_vars);
			ilog("Plugin '%s' initialize. ", item->GetName().c_str());
		}
	}

	void Application::Startup()
	{
		for (auto item : initialized_plugins_)
		{
			if (item)
			{

				item->Startup();
				startup_plugins_.push_back(item);
				ilog("Plugin '%s' startup. ", item->GetName().c_str());
			}
		}
	}

	void Application::Shutdown()
	{
		for (auto item : initialized_plugins_)
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
		service_face_->stop();
	}


	
	//--------------------------------------------------
	void Application::LoadCfgOptions()
	{		
		if (fs::exists(cfg_file_path_))
		{
			bpo::variables_map var_map;
			bpo::store(bpo::parse_config_file<char>(cfg_file_path_.make_preferred().string().c_str(), cfg_options_, true), var_map);
			bpo::notify(var_map);
		}
	}

	void Application::Loop()
	{
		std::shared_ptr<boost::asio::signal_set> sigint_set(new boost::asio::signal_set(*service_face_, SIGINT));
		sigint_set->async_wait([sigint_set, this](const boost::system::error_code& err, int num)
		{
			Quit();
			sigint_set->cancel();
		});

		std::shared_ptr<boost::asio::signal_set> sigterm_set(new boost::asio::signal_set(*service_face_, SIGTERM));
		sigterm_set->async_wait([sigterm_set, this](const boost::system::error_code& err, int num)
		{
			Quit();
			sigterm_set->cancel();
		});

		service_face_->run();

		Shutdown(); /// perform synchronous shutdown
	}

}