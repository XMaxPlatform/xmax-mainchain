/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include <pro/types/time.hpp>

#include <boost/asio/io_service.hpp>

#include <blockbuilder_plugin.hpp>

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

			TimeMicroseconds time(1000000);

			timer_.expires_from_now(boost::posix_time::microseconds(time.GetValue()));
			timer_.async_wait(std::bind(&BlockBuilderImpl::BuildBlock, this));

		}
		/**
		* generate a new block data
		*/
		void BuildBlock()
		{
			chain::IChainContext* chain_context = parent_->GetApp()->GetPlugin<BlockChainPlugin>().GetChain();

			chain_context->BuildBlock();

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