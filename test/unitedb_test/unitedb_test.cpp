#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>

#include <unitedb/unitedb.hpp>

using namespace unitedb;

enum EOBJType
{
	OBJType_test = 0,
};

DBOBJECT_CLASS(TestDBObject, OBJType_test)
{

};

struct by_id;
typedef DBTableDeclaration<
	TestDBObject,
	boost::multi_index::indexed_by<
		boost::multi_index::ordered_unique<boost::multi_index::tag<by_id>, boost::multi_index::member<DBObjectBase, ObjectIDCode, &DBObjectBase::id_>>
	>
> TestIdx;

typedef DBTable<TestIdx> TestTable;


BOOST_AUTO_TEST_SUITE(unitedb_suite)

BOOST_AUTO_TEST_CASE(db_test)
{
	unitedb::Database db(fs::current_path(), 1024 * 1024);

	db.InitTable<TestTable>();



}

BOOST_AUTO_TEST_SUITE_END()