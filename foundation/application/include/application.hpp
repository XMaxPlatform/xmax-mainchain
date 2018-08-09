/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unordered_map>
#include <pro/io/file_system.hpp>
#include <apps.hpp>
#include <pluginface.hpp>



namespace xmaxapp
{
	namespace bpo = boost::program_options;

	/**
	* Application instance. 
	* Use for Application and Plugin System.
	*/
	class Application : public ApplicationBase
	{
	public:
		Application();
		virtual ~Application();

		/**
		* Get the app service interface.
		* @return App service interface.
		*/
		virtual AppService* GetService() const override;

		/**
		* Get data dir.
		* @return data dir.
		*/
		virtual fs::path GetDataDir() const override;

		/**
		* Init a concrete plugin
		* @param[in] plugin_name the plugin name.
		*/
		void PluginToInit(const string& plugin_name);

		/**
		* Register plugin to the application
		* @template param: plugin class
		*/
		template <class PluginType>
		void RegisterPlugin() {
			PluginType::RegistSelf();
			PluginToInit(PluginType::PluginName());
		}

		/**
		* Init a concrete plugin
		* @param[in] argc number of args
		* @param[in] argv the content of params
		*/
		bool Initialize(int argc, char** argv);
		/**
		*	start the app
		*/
		void Startup();
		/**
		*	shutdown the app
		*/
		void Shutdown();
		/**
		*	Main loop of the logic
		*/
		void Loop();
		/**
		*	quit the app
		*/
		void Quit();


		//==============Utility methods ============

		void SetDefaultConfigFilePath(const fs::path& path) { cfg_file_path_ = path; }

	private:
		void SetupApplicationOptions();
		
		/**
		* Load all the options supported in the configuration file
		* @param[out] option variable map storing the option values
		*/
		void LoadCfgOptions(bpo::variables_map& var_map);

		/**
		*	Create default configuration file
		*/
		void CreateDefaultCfgFile();

	private:

		std::unordered_map<string, std::unique_ptr<PluginFace>>	pluginmap_;
		std::vector<PluginFace*>						initialized_plugins_;
		std::vector<PluginFace*>						startup_plugins_;

		OptionsDesc     app_options_;
		OptionsDesc     cfg_options_;

		fs::path cfg_file_path_;
		fs::path data_dir_{ "data-dir" };


		std::unique_ptr<AppService>  service_face_;	
	};


}
