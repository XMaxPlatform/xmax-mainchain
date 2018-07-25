/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <map>
#include <pro/io/file_system.hpp>
#include <apps.hpp>
#include <pluginface.hpp>



namespace xmaxapp
{
	namespace bpo = boost::program_options;

	/*! \brief Application instance.
	*         Brief description continued.
	*
	*  Use for Application and Plugin System.
	*/
	class Application : public ApplicationBase
	{
	public:
		Application();
		virtual ~Application();

		//! Get the app service interface.
		/*!
		\return App service interface.
		*/
		virtual AppService* GetService() const override;

		//! Get data dir.
		/*!
		\return data dir.
		*/
		virtual fs::path GetDataDir() const override;

		//! Init a concrete plugin
		/*!
		\param plugin_name the plugin name.
		*/
		void PluginToInit(const string& plugin_name);

		//! Init a concrete plugin
		/*!
		\param argc	number of args
		\param argv	the content of params
		\return If the program will continue running or exit directly
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
		void LoadCfgOptions(bpo::variables_map& var_map);
		void CreateDefaultCfgFile();

	private:

		std::map<string, std::unique_ptr<PluginFace>>	pluginmap_;
		std::vector<PluginFace*>						initialized_plugins_;
		std::vector<PluginFace*>						startup_plugins_;

		OptionsDesc     app_options_;
		OptionsDesc     cfg_options_;

		fs::path cfg_file_path_;
		fs::path data_dir_{ "data-dir" };

		std::unique_ptr<AppService>  service_face_;
	};


}
