#define BOOST_TEST_MODULE module_test

#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>
#include "script_module.h"
#include <pro/types/time.hpp>
#include <boost/test/included/unit_test.hpp>

using namespace xmaxapp;
using namespace xmax;
using namespace boost::unit_test;
using namespace pro;

//#define Test_Script

BOOST_AUTO_TEST_SUITE(time_test)

BOOST_AUTO_TEST_CASE(time_1)
{
	TimeMilliseconds milli(2500ll);
	TimeMicroseconds micro = milli.ToTime<TimeMicroseconds>();
	TimeMilliseconds milli2 = micro.ToTime<TimeMilliseconds>();

	TimeSeconds sec = micro.ToTime<TimeSeconds>();

	TimeMicroseconds micro2 = sec.ToTime<TimeMicroseconds>();
	TimeMilliseconds milli3 = sec.ToTime<TimeMilliseconds>();

	BOOST_CHECK(milli.GetValue() == 2500ll);
	BOOST_CHECK(micro.GetValue() == 2500ll * 1000ll);
	BOOST_CHECK(milli2.GetValue() == 2500ll);

	BOOST_CHECK(sec.GetValue() == 2ll);
	BOOST_CHECK(micro2.GetValue() == 2ll * 1000ll * 1000ll);
	BOOST_CHECK(milli3.GetValue() == 2ll * 1000ll);
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

#ifdef Test_Script
BOOST_AUTO_TEST_SUITE(vm_plugin_test)

BOOST_AUTO_TEST_CASE(bpt_1)
{
	xmax::scriptv8::ScriptMoudle::GetInstance().Init();
	xmax::scriptv8::ScriptMoudle::GetInstance().Discard();
}

BOOST_AUTO_TEST_SUITE_END()
#endif
