#define BOOST_TEST_MODULE module_test

#include <blockchain_plugin.hpp>
#include <blockbuilder_plugin.hpp>
#include "script_module.hpp"
#include "script_global_module.hpp"
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
 	xmax::scriptv8::ScriptGlobalMoudle::GetInstance().SetupV8Env();

	v8::Isolate::Scope isolate_scope(xmax::scriptv8::ScriptGlobalMoudle::GetInstance().GetIsolate());
	v8::HandleScope handle_scope(xmax::scriptv8::ScriptGlobalMoudle::GetInstance().GetIsolate());
	


	xmax::scriptv8::ScriptMoudle module;
	module.CleanInstrunction();
	module.LoadScript("function LoopCheckTest() { int i = 10; while ( i > 0 ) { i--; }");
	module.RunFoo("LoopCheckTest");
 	xmax::scriptv8::ScriptGlobalMoudle::GetInstance().DiscardV8Env();
}

BOOST_AUTO_TEST_SUITE_END()
#endif
