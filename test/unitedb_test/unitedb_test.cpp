#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>

#include <unitedb/unitedb.hpp>

using namespace unitedb;

enum EOBJType
{
	OBJType_test = 0,
};

class TestDBObject : public unitedb::DBObject<TestDBObject, OBJType_test>
{
public:
	int oid = 0;
};

typedef DBTableDeclaration<
	TestDBObject,
	boost::multi_index::indexed_by<
	boost::multi_index::ordered_non_unique<
	boost::multi_index::member<
	TestDBObject, int, &TestDBObject::oid
	>
	>
	>
> TestIdx;

typedef DBTable<TestIdx> TestTable;


BOOST_AUTO_TEST_SUITE(unitedb_suite)

BOOST_AUTO_TEST_CASE(db_test)
{
	unitedb::Database db(fs::current_path(), 1024 * 1024);

	db.InitTable<TestTable>();

	MappedString()

}

BOOST_AUTO_TEST_SUITE_END()