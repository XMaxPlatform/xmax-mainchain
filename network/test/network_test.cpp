#define BOOST_TEST_MODULE network_test


#include <boost/test/included/unit_test.hpp>
#include <test.pb.h>


BOOST_AUTO_TEST_SUITE(network_protobuf)

BOOST_AUTO_TEST_CASE(proto_set_int32_field)
{
	using namespace tutorial;
	Person person_msg;
	person_msg.set_id(12345);
	BOOST_CHECK(person_msg.id() == 12345);

}


BOOST_AUTO_TEST_SUITE_END()