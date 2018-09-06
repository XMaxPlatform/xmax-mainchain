#define BOOST_TEST_MODULE network_test


#include <boost/test/included/unit_test.hpp>
#include <google/protobuf/stubs/common.h>
#include <test.pb.h>
#include <any_test.pb.h>

using namespace tutorial;
using namespace google::protobuf;

namespace {
	// utility function generates data for testing
	static inline void FillTestDataSimple(Person& person) {
		person.set_email("12345@gmail.com");
		person.set_name("12345");
		person.set_id(12345);
	}
	// utility function compares data for checking
	static inline bool CompareSimpleTestData(const Person& lhs, const Person& rhs) {
		return (lhs.email() == rhs.email()) &&
			(lhs.id() == rhs.id()) &&
			(lhs.name() == rhs.name());
	}
}


BOOST_AUTO_TEST_SUITE(network_protobuf)

BOOST_AUTO_TEST_CASE(proto_set_int32_field)
{
	// set and check id
	Person person_msg;
	person_msg.set_id(12345);
	BOOST_CHECK(person_msg.id() == 12345);

}

BOOST_AUTO_TEST_CASE(proto_set_string_field)
{
	// set and check email
	Person person_msg;
	person_msg.set_email("example@example.com");	
	BOOST_CHECK(person_msg.email() == "example@example.com");
}


BOOST_AUTO_TEST_CASE(proto_simple_serialize_string1)
{
	// prepare data object
	Person person_msg;
	FillTestDataSimple(person_msg);
	// serialize to string
	std::string serialize_str;
	person_msg.SerializeToString(&serialize_str);
	// deserialize from former string
	Person ser_person_msg;
	ser_person_msg.ParseFromString(serialize_str);
	// compare deserialized object with origin data object
	BOOST_CHECK(CompareSimpleTestData(person_msg, ser_person_msg));
}

BOOST_AUTO_TEST_CASE(proto_simple_serialize_string2)
{
	// prepare data object
	Person person_msg;
	FillTestDataSimple(person_msg);
	// serialize to string
	std::string serialize_str = person_msg.SerializeAsString();
	// deserialize from former string
	Person ser_person_msg;
	ser_person_msg.ParseFromString(serialize_str);
	// compare deserialized object with origin data object
	BOOST_CHECK(CompareSimpleTestData(person_msg, ser_person_msg));
}

BOOST_AUTO_TEST_CASE(proto_simple_serialize_array)
{
	Person person_msg;
	FillTestDataSimple(person_msg);

	size_t data_size = person_msg.ByteSize();
	std::vector<char> data;
	data.resize(data_size);
	person_msg.SerializeToArray(static_cast<void*>(data.data()), data_size);

	Person ser_person_msg;
	ser_person_msg.ParseFromArray(static_cast<const void*>(data.data()), data_size);

	BOOST_CHECK(CompareSimpleTestData(person_msg, ser_person_msg));
}

BOOST_AUTO_TEST_CASE(proto_debug_string)
{
	Person person_msg;
	FillTestDataSimple(person_msg);

	string debug_msg = person_msg.DebugString();	

	bool c1 = debug_msg.find("id") != string::npos;
	bool c2 = debug_msg.find("name") != string::npos;
	bool c3 = debug_msg.find("email") != string::npos;

	BOOST_CHECK(c1 && c2 && c3);
}


BOOST_AUTO_TEST_CASE(proto_simple_testclear)
{
	using namespace google::protobuf::io;

	Person person_msg;
	std::string init_email = person_msg.email();
	person_msg.set_email("12345@gmail.com");
	person_msg.clear_email();

	BOOST_CHECK(init_email == person_msg.email());
}

BOOST_AUTO_TEST_CASE(proto_simple_serialize_stream) {
	Person person_msg;
	FillTestDataSimple(person_msg);

	std::stringstream ss;
	person_msg.SerializeToOstream(&ss);

	Person ser_person_msg;
	ser_person_msg.ParseFromIstream(&ss);

	BOOST_CHECK(CompareSimpleTestData(person_msg, ser_person_msg));
}



BOOST_AUTO_TEST_CASE(test_any_pack_unpack)
{	
	protobuf_unittest::TestAny submessage;
	submessage.set_int32_value(12345);
	protobuf_unittest::TestAny message;
	message.mutable_any_value()->PackFrom(submessage);

	string data = message.SerializeAsString();

	BOOST_ASSERT(message.ParseFromString(data));
	BOOST_CHECK(message.has_any_value());
	BOOST_ASSERT(message.any_value().UnpackTo(&submessage));
	BOOST_CHECK_EQUAL(12345, submessage.int32_value());

}

BOOST_AUTO_TEST_CASE(test_any_pack_unpack2)
{
	// We can pack a Any message inside another Any message.
	protobuf_unittest::TestAny submessage;
	submessage.set_int32_value(12345);
	google::protobuf::Any any;
	any.PackFrom(submessage);
	protobuf_unittest::TestAny message;
	message.mutable_any_value()->PackFrom(any);
	 
	string data = message.SerializeAsString();

	BOOST_ASSERT(message.ParseFromString(data));
	BOOST_CHECK(message.has_any_value());
	BOOST_ASSERT(message.any_value().UnpackTo(&any));
	BOOST_ASSERT(any.UnpackTo(&submessage));
	BOOST_CHECK_EQUAL(12345, submessage.int32_value());
}

BOOST_AUTO_TEST_CASE(test_any_testis) {
	protobuf_unittest::TestAny submessage;
	submessage.set_int32_value(12345);
	google::protobuf::Any any;
	any.PackFrom(submessage);
	BOOST_ASSERT(any.ParseFromString(any.SerializeAsString()));
	BOOST_CHECK(any.Is<protobuf_unittest::TestAny>());
	BOOST_CHECK(!any.Is<google::protobuf::Any>());

	protobuf_unittest::TestAny message;
	message.mutable_any_value()->PackFrom(any);
	BOOST_ASSERT(message.ParseFromString(message.SerializeAsString()));
	BOOST_CHECK(!message.any_value().Is<protobuf_unittest::TestAny>());
	BOOST_CHECK(message.any_value().Is<google::protobuf::Any>());
}

BOOST_AUTO_TEST_CASE(test_any_reflection_typename) {
	protobuf_unittest::TestAny submessage;
	std::string type_name = submessage.GetTypeName();
	BOOST_CHECK(type_name == "protobuf_unittest.TestAny");
}


BOOST_AUTO_TEST_SUITE_END()





namespace {

	using namespace google::protobuf;
	// Force shutdown at process exit so that we can test for memory leaks.  To
	// actually check for leaks, I suggest using the heap checker included with
	// google-perftools.  Set it to "draconian" mode to ensure that every last
	// call to malloc() has a corresponding free().
	struct ForceShutdown {
		~ForceShutdown() {
			ShutdownProtobufLibrary();
			// Test to shutdown the library twice, which should succeed.
			ShutdownProtobufLibrary();
		}
	} force_shutdown;

}  // namespace