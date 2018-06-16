#define BOOST_TEST_MODULE network_test


#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(network_test_suite01)

BOOST_AUTO_TEST_CASE(net_01)
{
	bool bret = true;
	BOOST_CHECK(bret == false);
}


BOOST_AUTO_TEST_SUITE_END()