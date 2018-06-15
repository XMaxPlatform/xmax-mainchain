#define BOOST_TEST_MODULE module_test

#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>

#include <pro/types/time.hpp>
#include <boost/test/included/unit_test.hpp>

using namespace xmaxapp;
using namespace xmax;
using namespace boost::unit_test;
using namespace pro;


BOOST_AUTO_TEST_SUITE(time_test)

BOOST_AUTO_TEST_CASE(time_1)
{
	TimeMilliseconds time(2000ll);
	TimeMicroseconds micro = time.ToTime<TimeMicroseconds>();

	BOOST_CHECK(time.GetValue() == 2000ll);
	BOOST_CHECK(micro.GetValue() == 2000ll * 1000ll);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE (blockchain_plugin_test)

BOOST_AUTO_TEST_CASE(bpt_1)
{
	BlockChainPlugin::RegistSelf();
	bool bret = BlockChainPlugin::RegistSelf();
	BOOST_CHECK(bret == false);
}

BOOST_AUTO_TEST_SUITE_END()

