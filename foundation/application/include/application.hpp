/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <map>
#include <pluginface.hpp>

//namespace boost {
//	namespace asio {
//
//#if !defined(BOOST_ASIO_NO_DEPRECATED)
//		/// Typedef for backwards compatibility.
//		class io_context;
//		typedef io_context io_service;
//#endif // !defined(BOOST_ASIO_NO_DEPRECATED)
//
//	} // namespace asio
//} // namespace boost

namespace xmaxapp
{
	class Application : public ApplicationBase
	{
	public:
		Application();
		virtual ~Application();

		// interface 
		AppService* GetService() const override;
		// ------------------

		void PluginToInit(const string& plugin_name);

		void Initialize(int argc, char** argv);

		void Startup();

		void Shutdown();

		void Loop();

		void Quit();

	private:

		std::map<string, std::unique_ptr<PluginFace>>	pluginmap_;
		std::vector<PluginFace*>						initialized_plugins_;
		std::vector<PluginFace*>						startup_plugins_;

		OptionsDesc     app_options_;
		OptionsDesc     cfg_options_;

		std::unique_ptr<AppService>  service_face_;
	};


}
