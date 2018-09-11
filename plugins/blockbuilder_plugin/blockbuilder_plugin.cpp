/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include <pro/types/time.hpp>
#include <pro/utils/time_utils.hpp>
#include <boost/asio/io_service.hpp>
#include <chain_global.hpp>
#include <blockbuilder_plugin.hpp>

namespace xmax
{
	using namespace pro;
	using namespace chain;
	/**
	* implementation of generating a block, using boost asio
	*/
	class BlockBuilderImpl
	{
	public:
		boost::asio::deadline_timer timer_;
		BlockBuilderPlugin* parent_ = nullptr;

		BlockBuilderImpl(boost::asio::io_service& io, BlockBuilderPlugin* p)
			: timer_(io)
			, parent_(p)
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

			// Next build time.
			// If we would wait less than "1/10 of block_interval", wait for the whole block interval.
			TimePoint now = utils::TimeNow();
			int64_t time_to_next_block_time = (ChainGlobal::ChainTimestampUnitUS) - (now.time_since_epoch().count() % (ChainGlobal::ChainTimestampUnitUS));
			if (time_to_next_block_time < ChainGlobal::MiniNextBlockUS) {     // we must sleep for at least 50ms
				Logf("Less than ${t}us to next block time, time_to_next_block_time ${bt}us",
					("t", ChainGlobal::ChainTimestampUnitUS)("bt", time_to_next_block_time));
				time_to_next_block_time += ChainGlobal::ChainTimestampUnitUS;
			}

			timer_.expires_from_now(boost::posix_time::microseconds(time_to_next_block_time));
			timer_.async_wait(std::bind(&BlockBuilderImpl::BuildBlock, this));

		}
		/**
		* generate a new block data
		*/
		void BuildBlock()
		{
			chain::IChainContext* chain_context = parent_->GetApp()->GetPlugin<BlockChainPlugin>().GetChain();

			chain_context->BuildBlock();

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
		impl_ = std::make_unique<BlockBuilderImpl>(*GetApp()->GetService(), this);
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