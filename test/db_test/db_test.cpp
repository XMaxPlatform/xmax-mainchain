#define BOOST_TEST_MODULE db_test

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>


#include <iostream>
#include <boost/test/included/unit_test.hpp>



using namespace std;


BOOST_AUTO_TEST_SUITE(mongo_db_suite)

const static std::string mongo_uri = "mongodb://localhost:27017";

BOOST_AUTO_TEST_CASE(test_mongo_db_connect) {
	
	try {
		mongocxx::uri uri = mongocxx::uri{ mongo_uri };
		mongocxx::client mongo_cli = mongocxx::client{ uri };
	}
	catch (mongocxx::exception& e) {
		cout << "Caught mongocxx exception:" << e.what() << endl;
		BOOST_CHECK(false);
	}

	BOOST_CHECK(true);			
}



BOOST_AUTO_TEST_SUITE_END()