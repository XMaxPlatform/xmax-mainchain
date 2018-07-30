#define BOOST_TEST_MODULE module_test

#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>
#include "script_module.hpp"

#include <boost/test/included/unit_test.hpp>
#include "pro/utils/singleton.hpp"

using namespace xmaxapp;
using namespace xmax;
using namespace boost::unit_test;


#define Test_Script



#ifdef Test_Script
BOOST_AUTO_TEST_SUITE(vm_plugin_test)

BOOST_AUTO_TEST_CASE(bpt_1)
{
 	xmax::scriptv8::ScriptMoudle::GetInstance().Init();
// 	xmax::scriptv8::ScriptMoudle::GetInstance().Call("function test(){return 3;}", "test");
// 
// 	xmax::scriptv8::ScriptMoudle::GetInstance().Call("function test2(){return 4;}", "test2");
// 	//v8::String::Utf8Value utf8(xmax::scriptv8::ScriptMoudle::GetInstance().GetIsolate(), result);
// 
// 	//printf("%s\n", *utf8);
 	xmax::scriptv8::ScriptMoudle::GetInstance().Discard();
}

BOOST_AUTO_TEST_SUITE_END()
#endif
