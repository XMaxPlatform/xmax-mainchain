#define BOOST_TEST_MODULE network_test


#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(network_protobuf)

BOOST_AUTO_TEST_CASE(simple_proto_case)
{
	bool bret = true;
	BOOST_CHECK(bret == false);
}


BOOST_AUTO_TEST_SUITE_END()