/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include <boost/asio/io_service.hpp>
#include "blockbuilder_plugin.hpp"

namespace xmax
{
	class BlockBuilderImpl
	{
	public:
		boost::asio::deadline_timer timer;


		BlockBuilderImpl(boost::asio::io_service& io)
			: timer(io)
		{

		}
	};

	BlockBuilderPlugin::BlockBuilderPlugin()
	{

	}

	void BlockBuilderPlugin::Initialize(const VarsMap& options)
	{
		Super::Initialize(options);

		impl_ = std::make_unique<BlockBuilderImpl>(*GetApp()->GetService());
	}

	void BlockBuilderPlugin::Startup()
	{
		Super::Startup();
	}

	void BlockBuilderPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		ilog("Blockbuilder plugin init options.");
	}

}