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
	template<typename T>
	TestDBObject(DBAlloc<T>)
	{

	}

	int tval = 5;
};

struct by_id;
typedef DBTableDeclaration<
	TestDBObject,
	boost::multi_index::indexed_by<
	INDEXED_BY_OBJECT_ID
	>
> TestIdx;

typedef DBTable<TestIdx> TestTable;


BOOST_AUTO_TEST_SUITE(unitedb_suite)

BOOST_AUTO_TEST_CASE(db_develop_test)
{
	std::unique_ptr<unitedb::Database> db(unitedb::Database::InitDB(fs::current_path(), 1024 * 1024, unitedb::Database::Discard));//);//

	db->InitTable<TestTable>();

	auto tbl = db->GetTable<TestTable>();

	auto val1 = tbl->NewObject([&](TestDBObject& a)
	{
		a.tval = 1024;
	});

	BOOST_CHECK(val1->tval == 1024);

	auto id1 = val1->GetID();

	auto val1_a = tbl->FindObject<ByObjectID>(id1);

	BOOST_CHECK(val1_a->tval == 1024);

	auto undo = db->StartUndo();

	tbl->UpdateObject(val1_a, [&](TestTable::ObjectType& a)
	{
		a.tval = 1025;
	});
	val1_a = tbl->FindObject<ByObjectID>(id1);

	BOOST_CHECK(tbl->FindObject<ByObjectID>(id1)->tval == 1025);

	undo.Undo();


	BOOST_CHECK(tbl->FindObject<ByObjectID>(id1)->tval == 1024);

	const auto& ids = tbl->GetOrderIndex<ByObjectID>();
	std::vector<TestDBObject> objs;
	for (auto it : ids)
	{
		objs.push_back(it);
	}




	//==========================================================

	TestIdx tidxs(db->GetSegment());

	TestIdx::index<ByObjectID>::type& tp = tidxs.get<ByObjectID>();

	tidxs.emplace([&](TestTable::ObjectType& a)
	{
	}, TestTable::AllocType(db->GetSegment()));

	auto& idx = tidxs.get<ByObjectID>();

	auto ff = idx.find(0);

	idx.iterator_to(*ff);

	tidxs.modify(ff, [&](TestTable::ObjectType& a)
	{
	});
	
	auto it = tidxs.erase(ff);


	int ss = 0;
	//tidxs.get<0>().emplace(TestDBObject());
}

BOOST_AUTO_TEST_SUITE_END()