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
		template<typename TPlugin>
		void regist_plugin()
		{
			if (!check_regist(TPlugin::plugin_name()) )
			{
				regist_plugin(TPlugin::get_plugin_factory());
			}			
		}
		void initialize(int argc, char** argv);
		void startup();
		void shutdown();

		void loop();
		void quit();

	private:


		void regist_plugin(plugin_factory&& _factory);

		bool check_regist(const string& name) const;

		std::map<string, plugin_factory>			plugin_factorys;
		std::map<string, std::unique_ptr<plugin_face>>	pluginmap;
		std::vector<plugin_face*>						initialized_plugins;
		std::vector<plugin_face*>						startup_plugins;

		options_desc     app_options;
		options_desc     cfg_options;

		std::unique_ptr<app_service>  service_face;
	};


}
