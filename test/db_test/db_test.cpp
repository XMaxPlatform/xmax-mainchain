#define BOOST_TEST_MODULE db_test

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>

#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <iostream>
#include <boost/test/included/unit_test.hpp>



using namespace std;


BOOST_AUTO_TEST_SUITE(mongo_db_suite)

const static std::string mongo_uri = "mongodb://localhost:27017";
const static std::string mongo_uri_nonexist = "mongodb://localhost:27018";
const static std::string db_name = "testdb";
const static std::string collection_name = "testcol";

static mongocxx::instance instance{};

static inline bool IsConnectionEx(const mongocxx::exception& ex) {
	std::string ex_msg = ex.what();
	return ex_msg.find("No suitable servers found") != std::string::npos;
}

BOOST_AUTO_TEST_CASE(test_mongo_db_connect_fail) {
	
	BOOST_CHECK_EXCEPTION({
		mongocxx::uri uri = mongocxx::uri{ mongo_uri_nonexist };
		mongocxx::client mongo_cli = mongocxx::client{ uri };
		mongocxx::database db = mongo_cli[db_name];
		mongocxx::collection col = db[collection_name];
		bsoncxx::builder::stream::document doc{};
		doc << "name" << "abc";
		col.insert_one(doc.view());
		col.create_index(bsoncxx::from_json(R"foo({ "name" : 1 })foo"));
		}, mongocxx::exception, 
		IsConnectionEx);
	
}

static void TestDbConnect() {
	mongocxx::uri uri = mongocxx::uri{ mongo_uri };
	mongocxx::client mongo_cli = mongocxx::client{ uri };
	mongocxx::database db = mongo_cli[db_name];
	mongocxx::collection col = db[collection_name];
	bsoncxx::builder::stream::document doc{};
	doc << "name" << "abc";
	col.insert_one(doc.view());
	col.create_index(bsoncxx::from_json(R"foo({ "name" : 1 })foo"));
}

BOOST_AUTO_TEST_CASE(test_mongo_db_connect) {

	try
	{
		mongocxx::uri uri = mongocxx::uri{ mongo_uri.c_str() };
		mongocxx::client mongo_cli = mongocxx::client{ uri };
		mongocxx::database db = mongo_cli[db_name];
		db.create_collection(collection_name);		
	}
	catch (const mongocxx::exception& e)
	{

		BOOST_CHECK(false);
	}
	
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_mongo_db_write_col) {
	mongocxx::uri uri = mongocxx::uri{ mongo_uri.c_str() };
	mongocxx::client mongo_cli = mongocxx::client{ uri };
	mongocxx::collection col = mongo_cli[db_name][collection_name];
	col.create_index(bsoncxx::from_json(R"foo({ "name" : 1 })foo"));

}
BOOST_AUTO_TEST_CASE(test_mongo_db_drop_col) {
	mongocxx::uri uri = mongocxx::uri{ mongo_uri.c_str() };
	mongocxx::client mongo_cli = mongocxx::client{ uri };
	mongocxx::collection col = mongo_cli[db_name][collection_name];
	col.drop();
}



BOOST_AUTO_TEST_SUITE_END()