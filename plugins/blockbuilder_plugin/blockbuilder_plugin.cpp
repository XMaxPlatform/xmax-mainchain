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
	/**
	* implementation of generating a block, using boost asio
	*/
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

			NextBlock();
		}
		/**
		* next time interval of build a new block
		*/
		void NextBlock() 
		{

			TimeMicroseconds time(1000000);
			timer_.expires_from_now(boost::posix_time::microseconds(time.GetValue()));
			timer_.async_wait(std::bind(&BlockBuilderImpl::BuildBlock, this));

		}
		/**
		* generate a new block data
		*/
		void BuildBlock()
		{
			Logf("building block.");
			NextBlock();
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

	}

}