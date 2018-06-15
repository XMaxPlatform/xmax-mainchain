#define BOOST_TEST_MODULE module_test

#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>
#include<boost/test/included/unit_test.hpp>

using namespace xmaxapp;
using namespace xmax;
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE (blockchain_plugin_test)

BOOST_AUTO_TEST_CASE(bpt_1)
{
	BlockChainPlugin::RegistSelf();
	bool bret = BlockChainPlugin::RegistSelf();
	BOOST_CHECK(bret == false);
}

BOOST_AUTO_TEST_SUITE_END()

