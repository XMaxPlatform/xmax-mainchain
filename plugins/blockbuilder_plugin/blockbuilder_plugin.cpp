/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include <pro/types/time.hpp>
#include <boost/asio/io_service.hpp>
#include "blockbuilder_plugin.hpp"

namespace xmax
{
	using namespace pro;
	class BlockBuilderImpl
	{
	public:
		boost::asio::deadline_timer timer_;


		BlockBuilderImpl(boost::asio::io_service& io)
			: timer_(io)
		{

		}

		void StartLoop()
		{
			//timer_.expires_from_now(boost::posix_time::microseconds(time_to_next_block_time));
			//timer_.async_wait(boost::bind(&BlockBuilderImpl::NextBlock, this));

			//TimeMilliseconds time(2000ll);
			//TimeMicroseconds micro = time.ToTime<TimeMicroseconds>();
			//TimeMilliseconds time2 = micro.ToTime<TimeMilliseconds>();

			assert(time.GetValue() == 2000ll);
			assert(micro.GetValue() == 2000ll * 1000ll);

		}

		void BlockBuilderImpl::NextBlock() {



		}

	};

	BlockBuilderPlugin::BlockBuilderPlugin()
	{

	}

	void BlockBuilderPlugin::OnCreated()
	{
		Super::OnCreated();
		impl_ = std::make_unique<BlockBuilderImpl>(*GetApp()->GetService());
	}

	void BlockBuilderPlugin::Initialize(const VarsMap& options)
	{
		Super::Initialize(options);
	}

	void BlockBuilderPlugin::Startup()
	{
		Super::Startup();
		impl_->StartLoop();
	}

	void BlockBuilderPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{
		ilog("Blockbuilder plugin init options.");
	}

}