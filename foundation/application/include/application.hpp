/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <map>
#include <pluginface.hpp>

namespace boost {
	namespace asio {

#if !defined(BOOST_ASIO_NO_DEPRECATED)
		/// Typedef for backwards compatibility.
		class io_context;
		typedef io_context io_service;
#endif // !defined(BOOST_ASIO_NO_DEPRECATED)

	} // namespace asio
} // namespace boost

namespace Xmax
{
	using boost::program_options::options_description;

	class application
	{
	public:
		application();
		virtual ~application();
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

		void exec();
		void quit();
	private:


		void regist_plugin(plugin_factory&& _factory);

		bool check_regist(const string& name) const;

		std::map<string, plugin_factory> plugin_factorys;
		std::map<string, std::unique_ptr<iplugin>> pluginmap;
		std::vector<iplugin*>                  initialized_plugins;
		std::vector<iplugin*>                  startup_plugins;

		options_description     app_options;
		options_description     cfg_options;

		std::shared_ptr<boost::asio::io_service>  asio_service;
	};


}
