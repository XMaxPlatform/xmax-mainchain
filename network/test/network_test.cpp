#define BOOST_TEST_MODULE network_test


#include <boost/test/included/unit_test.hpp>
#include <google/protobuf/stubs/common.h>
#include <test.pb.h>

using namespace tutorial;

BOOST_AUTO_TEST_SUITE(network_protobuf)

BOOST_AUTO_TEST_CASE(proto_set_int32_field)
{
	
	Person person_msg;
	person_msg.set_id(12345);
	BOOST_CHECK(person_msg.id() == 12345);

}

BOOST_AUTO_TEST_CASE(proto_set_string_field)
{
	Person person_msg;
	person_msg.set_email("12345");	
	BOOST_CHECK(person_msg.email() == "12345");
}


BOOST_AUTO_TEST_SUITE_END()




namespace {

	using namespace google::protobuf;
	// Force shutdown at process exit so that we can test for memory leaks.  To
	// actually check for leaks, I suggest using the heap checker included with
	// google-perftools.  Set it to "draconian" mode to ensure that every last
	// call to malloc() has a corresponding free().
	struct ForceShutdown {
		~ForceShutdown() {
			ShutdownProtobufLibrary();
			// Test to shutdown the library twice, which should succeed.
			ShutdownProtobufLibrary();
		}
	} force_shutdown;

}  // namespace