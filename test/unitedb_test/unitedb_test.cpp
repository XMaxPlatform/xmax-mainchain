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
public:
	template<typename C, typename T>
	TestDBObject( C&& c, DBAlloc<T>)
	{
		c(*this);
	}
	TestDBObject() = default;

};

struct by_id;
typedef DBTableDeclaration<
	TestDBObject,
	boost::multi_index::indexed_by<
	INDEXED_BY_ID
	>
> TestIdx;

typedef DBTable<TestIdx> TestTable;


BOOST_AUTO_TEST_SUITE(unitedb_suite)

BOOST_AUTO_TEST_CASE(db_test)
{
	unitedb::Database db(fs::current_path(), 1024 * 1024);

	db.InitTable<TestTable>();

	db.NewObject<TestTable>([&](TestTable::ObjectType& a)
	{
		int xx = a.id_;
	});

	TestIdx tidxs(db.GetSegmentManager());

	tidxs.emplace([&](TestTable::ObjectType& a)
	{
		int xx = a.id_;
	}, TestTable::AllocType(db.GetSegmentManager()));

	//tidxs.get<0>().emplace(TestDBObject());
}

BOOST_AUTO_TEST_SUITE_END()