#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>

#include <unitedb/unitedb.hpp>

using namespace unitedb;



class TestDBObject : public unitedb::DBObject<5>
{
public:
	int oid = 0;
};

typedef boost::multi_index::multi_index_container<
	TestDBObject,
	boost::multi_index::indexed_by<
	boost::multi_index::ordered_non_unique<
	boost::multi_index::member<
	TestDBObject, int, &TestDBObject::oid
	>
	>
	>
> TestTable;


BOOST_AUTO_TEST_SUITE(unitedb_suite)

BOOST_AUTO_TEST_CASE(db_test)
{
	unitedb::Database db(fs::current_path(), 1024 * 1024);

}

BOOST_AUTO_TEST_SUITE_END()