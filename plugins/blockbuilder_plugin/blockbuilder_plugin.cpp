/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#include <pro/log/log.hpp>
#include <pro/types/time.hpp>
#include <unitedb/unitedb.hpp>

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

		std::unique_ptr<unitedb::Database> unitedb_;


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

		auto db_size = options.at("db-runtime-size").as<uint64_t>();


		impl_->unitedb_.reset(unitedb::Database::InitDB(GetApp()->GetDataDir(), db_size * SIZE_MB));


	}

	void BlockBuilderPlugin::Startup()
	{
		Super::Startup();
		impl_->StartLoop();
	}

	void BlockBuilderPlugin::InitOptions(OptionsDesc& cli, OptionsDesc& cfg)
	{

		cfg.add_options()
			("db-runtime-size", xmaxapp::options::value<uint64_t>()->default_value(1024), "size(in mb) of db memory.");

	}

}