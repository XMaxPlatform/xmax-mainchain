/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include <boost/program_options.hpp>
#include "application.hpp"
#include <fstream>
#include <iostream>
#include <unordered_map>


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

	fs::path Application::GetDataDir() const {
		return data_dir_;
	}

	Plugin* Application::FindPlugin(const string& name) const
	{
		auto itr = pluginmap_.find(name);
		if (itr != pluginmap_.end())
		{
			return itr->second.get();
		}
	}

	Plugin& Application::GetPlugin(const string& name) const
	{
		auto ptr = FindPlugin(name);
		if (!ptr)
			BOOST_THROW_EXCEPTION(std::runtime_error("unable to find plugin: " + name));
		return *ptr;
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
				Warnf("Unknown plugin:'${name}'.", ("name", plugin_name));
			}
		}
		else
		{
			Warnf("Plugin:'${name}' had in the init list.", ("name", plugin_name));
		}

	}
	void Application::InitalPlugin(const string& plugin_name)
	{
		initalplugins_.insert(plugin_name);
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

		app_cmd_opts.add_options()
			("help,h", "Show the command line options description and usage")
			("config-path", xpo::value<fs::path>(), "Application configuration file path.");

		app_cfg_opts.add_options()			
			("plugin", options::value< std::vector<string> >()->composing(), "Plugin(s) to startup.");

		cfg_options_.add(app_cfg_opts);
		app_options_.add(app_cfg_opts);
		app_options_.add(app_cmd_opts);	
	}


	bool Application::Initialize(int argc, char** argv)
	{
		
		SetupApplicationOptions();

		// parse options.
		variables_map option_vars;

		//Load command line options first
		options::store(options::parse_command_line(argc, argv, app_options_), option_vars);
		xpo::notify(option_vars);

		if (option_vars.count("help")) {
			std::cout << app_options_ << std::endl;
			return false;
		}

		LoadCfgOptions(option_vars);

		std::unordered_map<string, PluginFace*> added_plugins;

		auto add_plugin_visitor = [&added_plugins, this](auto& plugin_name) {
			if (added_plugins.find(plugin_name) == added_plugins.end()) {
				PluginFace* plugin = PluginFactory::NewPlugin(plugin_name, this);
				assert(plugin);
				pluginmap_[plugin_name].reset(plugin);
				initialized_plugins_.push_back(plugin);
				added_plugins[plugin_name] = plugin;
			}
		};
		// merge inital plugin.
		if (option_vars.count("plugin")) {
			auto plugin_ops = option_vars.at("plugin").as<std::vector<std::string>>();

			for (const auto& plugin_name : plugin_ops) {
				initalplugins_.insert(plugin_name);
			}
		}

		for (const auto& plugin_name : initalplugins_) {
			auto it = pluginmap_.find(plugin_name);
			if (it != pluginmap_.end()) {
				auto factory = PluginFactory::GetPluginFactory(plugin_name);
				assert(factory);
				factory->VisitDependentPluginsRecursively(add_plugin_visitor);
				add_plugin_visitor(plugin_name);
			}
		}
	


		if (option_vars.count("data-dir")) {
			fs::path data_dir = option_vars["data-dir"].as<fs::path>();
			if (data_dir.is_relative())
				data_dir = fs::current_path() / data_dir;
			data_dir_ = data_dir;
		}
		else
		{
			data_dir_ = fs::current_path() / data_dir_;
		}

		// 
		for (auto item : initialized_plugins_)
		{
			item->Initialize(option_vars);
			Logf("Plugin '${name}' initialize. ", ("name", item->GetName()));
		}

		return true;
	}

	void Application::Startup()
	{
		for (auto item : initialized_plugins_)
		{
			if (item)
			{
				item->Startup();
				startup_plugins_.push_back(item);
				Logf("Plugin '${name}' startup. ", ("name", item->GetName()) );
			}
		}
	}

	void Application::Shutdown()
	{
		for (auto item : initialized_plugins_)
		{
			if (item)
			{
				Logf("Plugin '${name}' shutdown. ", ("name", item->GetName()) );

				item->Shutdown();
			}
		}	
		initialized_plugins_.clear();
		startup_plugins_.clear();
		pluginmap_.clear();

		service_face_.reset();
	}

	void Application::Quit()
	{
		if (service_face_)
		{
			service_face_->stop();

		}
	}


	static inline std::string ExtractDefaultValue(const std::string& from_format_param) {
		auto start = from_format_param.find("(=") + 2;
		auto end = from_format_param.rfind(")");
		return from_format_param.substr(start, end - start);
	}

	//--------------------------------------------------
	void Application::CreateDefaultCfgFile()
	{
		assert(cfg_file_path_.has_filename());

		fs::path parent_path = cfg_file_path_.parent_path();
		if (!fs::exists(parent_path)) {
			fs::create_directories(parent_path);
		}	

		std::ofstream of(cfg_file_path_);
		for (auto op : cfg_options_.options()) {
			//Output description
			const xpo::option_description& od = *op;
			if (!od.description().empty()) {
				of << "#" << od.description() << std::endl;
			}


			boost::any value;
			if (!od.semantic()->apply_default(value)) {
				of << "#" << od.long_name() << " = " << std::endl;
			}
			else {				
				of << od.long_name() << " = " << ExtractDefaultValue(od.format_parameter()) << std::endl;
			}
			of << std::endl;
		}
		
		of.close();
	}


	//--------------------------------------------------
	void Application::LoadCfgOptions(xpo::variables_map& var_map)
	{		
		if (var_map.count("config-path")) {
			cfg_file_path_ = var_map.at("config-path").as<fs::path>();
		}

		if (!fs::exists(cfg_file_path_))
		{		
			CreateDefaultCfgFile();
		}

		xpo::store(xpo::parse_config_file<char>(cfg_file_path_.make_preferred().string().c_str(), cfg_options_, true), var_map);
		xpo::notify(var_map);
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