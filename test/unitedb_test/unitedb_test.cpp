#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>

#include <unitedb/unitedb.hpp>

using namespace unitedb;

enum EOBJType
{
	OBJ_TestA = 0,
	OBJ_TestB
};

DBOBJ_CLASS(DBTestA, OBJ_TestA)
{		
public:

	DBOBJ_BODY(DBTestA,
		(DB_FIELD(int, tval, 5))
	)

};

struct by_id;
typedef DBTableDeclaration<
	DBTestA,
	boost::multi_index::indexed_by<
	INDEXED_BY_OBJECT_ID
	>
> TestAIdx;


typedef DBTable<TestAIdx> TestATable;

DBOBJ_CLASS(DBTestB, OBJ_TestB)
{
	DBOBJ_BODY(DBTestB,
		(DB_FIELD(int, xx, 5))
		(DB_MFIELD(MVector<int>, arr))
	)

};


struct by_id;
typedef DBTableDeclaration<
	DBTestB,
	boost::multi_index::indexed_by<
	INDEXED_BY_OBJECT_ID
	>
> TestBIdx;

typedef DBTable<TestBIdx> TestBTable;

BOOST_AUTO_TEST_SUITE(unitedb_suite)

BOOST_AUTO_TEST_CASE(db_develop_test)
{
	std::unique_ptr<unitedb::Database> db(unitedb::Database::InitDB(fs::current_path(), 1024 * 1024, unitedb::Database::Discard));//);//

	db->InitTable<TestATable>();

	auto tbl = db->GetTable<TestATable>();

	//==========================================================

	TestAIdx tidxs(db->GetSegment());

	TestAIdx::index<ByObjectID>::type& tp = tidxs.get<ByObjectID>();

	tidxs.emplace([&](TestATable::ObjectType& a)
	{
	}, TestATable::AllocType(db->GetSegment()));


	TestBIdx tidxsb(db->GetSegment());
	auto& xx = tidxsb.emplace([&](TestBTable::ObjectType& a)
	{
		a.xx = 3;
		a.arr.push_back(10);
	}, TestBTable::AllocType(db->GetSegment()));


	auto& idx = tidxs.get<ByObjectID>();

	auto ff = idx.find(0);

	idx.iterator_to(*ff);

	tidxs.modify(ff, [&](TestATable::ObjectType& a)
	{
	});
	
	auto it = tidxs.erase(ff);


	int ss = 0;
	//tidxs.get<0>().emplace(TestDBObject());
}


BOOST_AUTO_TEST_CASE(db_op_test)
{
	std::unique_ptr<unitedb::Database> db(unitedb::Database::InitDB(fs::current_path(), 1024 * 1024, unitedb::Database::Discard));//);//

	db->InitTable<TestATable>();

	auto tbl = db->GetTable<TestATable>();

	// test new object.
	auto val1 = tbl->NewObject([&](DBTestA& a)
	{
		a.tval = 123;
	});

	BOOST_CHECK(val1->tval == 123);

	// test find object.
	auto id1 = val1->GetID();

	auto val1_a = tbl->FindObject<ByObjectID>(id1);

	BOOST_CHECK(val1_a->tval == 123);

	// test update object.
	tbl->UpdateObject(val1_a, [&](TestATable::ObjectType& a)
	{
		a.tval = 456;
	});

	BOOST_CHECK(tbl->FindObject<ByObjectID>(id1)->tval == 456);

	// test delect object.

	auto valdel = tbl->NewObject([&](DBTestA& a)
	{
		a.tval = 789;
	});
	auto iddel = valdel->GetID();
	tbl->DeleteObject(valdel);

	BOOST_CHECK(tbl->FindObject<ByObjectID>(iddel).Empty());
}


BOOST_AUTO_TEST_CASE(db_undo_test)
{
	std::unique_ptr<unitedb::Database> db(unitedb::Database::InitDB(fs::current_path(), 1024 * 1024, unitedb::Database::Discard));//);//

	db->InitTable<TestATable>();

	auto tbl = db->GetTable<TestATable>();


	const int tval_1_c = 100;
	const int tval_2_c = 200;

	const int tval_1_u = 110;
	const int tval_2_u = 210;


	auto val1 = tbl->NewObject([&](DBTestA& a)
	{
		a.tval = tval_1_c;
	});
	auto val2 = tbl->NewObject([&](DBTestA& a)
	{
		a.tval = tval_2_c;
	});

	auto id1 = val1->GetID();
	auto id2 = val2->GetID();


	auto undo_c = db->StartUndo(); // for cancel.


	tbl->UpdateObject(val1, [&](TestATable::ObjectType& a)
	{
		a.tval = tval_1_u;
	});

	tbl->UpdateObject(val2, [&](TestATable::ObjectType& a)
	{
		a.tval = tval_2_u;
	});

	auto undo_u = db->StartUndo(); // for undo.

	// update val1;
	tbl->UpdateObject(val1, [&](TestATable::ObjectType& a)
	{
		a.tval = 112;
	});
	// delete val2;
	auto fval2 = tbl->FindObject(id2);

	tbl->DeleteObject(fval2);

	// new val3

	auto val3 = tbl->NewObject([&](DBTestA& a)
	{
		a.tval = 333;
	});
	auto id3 = val3->GetID();

	// update val1;
	tbl->UpdateObject(val1, [&](TestATable::ObjectType& a)
	{
		a.tval = 11244;
	});

	// update val3;
	tbl->UpdateObject(val3, [&](TestATable::ObjectType& a)
	{
		a.tval = 33344;
	});

	// new val4
	auto val4 = tbl->NewObject([&](DBTestA& a)
	{
		a.tval = 444;
	});

	// delect v3
	tbl->DeleteObject(val3);

	// check undo_u.
	undo_u.Undo();

	BOOST_CHECK(tbl->FindObject<ByObjectID>(id1).Valid());
	BOOST_CHECK(tbl->FindObject<ByObjectID>(id2).Valid());
	BOOST_CHECK(tbl->FindObject<ByObjectID>(id3).Empty());

	BOOST_CHECK(tbl->FindObject<ByObjectID>(id1)->tval == tval_1_u);
	BOOST_CHECK(tbl->FindObject<ByObjectID>(id2)->tval == tval_2_u);


	// check undo_c
	undo_c.Cancel();

	// cancel do nothing, so check again.
	BOOST_CHECK(tbl->FindObject<ByObjectID>(id1).Valid());
	BOOST_CHECK(tbl->FindObject<ByObjectID>(id2).Valid());
	BOOST_CHECK(tbl->FindObject<ByObjectID>(id3).Empty());

	BOOST_CHECK(tbl->FindObject<ByObjectID>(id1)->tval == tval_1_u);
	BOOST_CHECK(tbl->FindObject<ByObjectID>(id2)->tval == tval_2_u);

	const auto& ids = tbl->GetOrderIndex<ByObjectID>();
	std::vector<DBTestA> objs;
	for (auto it : ids)
	{
		objs.push_back(it);
	}

}

BOOST_AUTO_TEST_CASE(db_commit_test)
{
	std::unique_ptr<unitedb::Database> db(unitedb::Database::InitDB(fs::current_path(), 1024 * 1024, unitedb::Database::Discard));//);//

	db->InitTable<TestATable>();

	auto tbl = db->GetTable<TestATable>();

	static const int cvala = 1000;
	static const int cvalb = 2000;
	static const int cvalc = 3000;

	auto undo_a = db->StartUndo(); //
	DBRevision reva = undo_a.GetRevision();
	DBRevision revdba = db->GetTopRevision();
	BOOST_CHECK(reva == 0 && revdba == 0);

	auto tval = tbl->NewObject([&](DBTestA& a)
	{
		a.tval = cvala;
	});

	auto tid = tval->GetID();

	auto undo_b = db->StartUndo(); //
	DBRevision revb = undo_b.GetRevision();
	DBRevision revdbb = db->GetTopRevision();
	BOOST_CHECK(revb == 1 && revdbb == 1);

	tbl->UpdateObject(tval, [&](DBTestA& a)
	{
		a.tval = cvalb;
	});

	undo_b.Combine();
	BOOST_CHECK(!undo_b.Valid());
	BOOST_CHECK(db->GetTopRevision() == 0);

	auto undo_c = db->StartUndo(); //
	DBRevision revc = undo_c.GetRevision();
	DBRevision revdbc = db->GetTopRevision();
	BOOST_CHECK(revc == 1 && revdbc == 1);

	tbl->UpdateObject(tval, [&](DBTestA& a)
	{
		a.tval = cvalc;
	});

	BOOST_CHECK(db->GetLastCommit() == InvalidRevision);
	db->Commit(revdba);
	undo_c.Undo();

	BOOST_CHECK(db->GetLastCommit() == revdba);
	BOOST_CHECK(tbl->FindObject(tid)->tval == cvalb);
}


BOOST_AUTO_TEST_SUITE_END()