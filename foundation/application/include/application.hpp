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

namespace Xmaxapp
{
	class application : public appbase
	{
	public:
		application();
		virtual ~application();

		// interface 
		app_service* get_service() const override;
		// ------------------

		void plugin_to_init(const string& plugin_name);

		void initialize(int argc, char** argv);

		void startup();
		void shutdown();

		void loop();
		void quit();

	private:

		std::map<string, std::unique_ptr<plugin_face>>	pluginmap;
		std::vector<plugin_face*>						initialized_plugins;
		std::vector<plugin_face*>						startup_plugins;

		options_desc     app_options;
		options_desc     cfg_options;

		std::unique_ptr<app_service>  service_face;
	};


}
