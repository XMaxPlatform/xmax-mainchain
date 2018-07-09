#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>

#include <unitedb/unitedb.hpp>

using namespace unitedb;



class TestDBObject : public unitedb::DBObject<5>
{

};



BOOST_AUTO_TEST_SUITE(unitedb_suite)

BOOST_AUTO_TEST_CASE(db_test)
{


}

BOOST_AUTO_TEST_SUITE_END()