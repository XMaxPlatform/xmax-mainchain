#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>
#include <pro/utils/singleton.hpp>
#include <pro/types/time.hpp>
#include <pro/types/any_value.hpp>

using namespace pro;

BOOST_AUTO_TEST_SUITE(pro_utils_suite)

namespace {
	class TestSingleton : public pro::Singleton<TestSingleton> {
		DECLARE_USE_SINGLETON(TestSingleton);

	public:
		void SetA(int a) {
			a_ = a;
		}

		int GetA() const { return a_; }

	private:
		int a_{ 0 };
	};

	TestSingleton::TestSingleton() {}
	TestSingleton::~TestSingleton() {}
}

BOOST_AUTO_TEST_CASE(test_singleton) {
	TestSingleton& s1 = TestSingleton::GetInstance();
	TestSingleton::GetInstance().SetA(12345);
	BOOST_CHECK(s1.GetA() == 12345);
}

BOOST_AUTO_TEST_SUITE(pro_time_test)

BOOST_AUTO_TEST_CASE(pro_time_1)
{
	TimeMilliseconds milli(2500ll);
	TimeMicroseconds micro = milli.ToTime<TimeMicroseconds>();
	TimeMilliseconds milli2 = micro.ToTime<TimeMilliseconds>();

	TimeSeconds sec = micro.ToTime<TimeSeconds>();

	TimeMicroseconds micro2 = sec.ToTime<TimeMicroseconds>();
	TimeMilliseconds milli3 = sec.ToTime<TimeMilliseconds>();

	BOOST_CHECK(milli.GetValue() == 2500ll);
	BOOST_CHECK(micro.GetValue() == 2500ll * 1000ll);
	BOOST_CHECK(milli2.GetValue() == 2500ll);

	BOOST_CHECK(sec.GetValue() == 2ll);
	BOOST_CHECK(micro2.GetValue() == 2ll * 1000ll * 1000ll);
	BOOST_CHECK(milli3.GetValue() == 2ll * 1000ll);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(any_value)

BOOST_AUTO_TEST_CASE(any_value)
{
	pro::AnyValue anyi32 = (int32_t)53;
	pro::AnyValue anyi64 = (int64_t)64;
	pro::AnyValue anystr = "test string";

	int32_t c = anyi32.CastTo<int32_t>();
	BOOST_CHECK(c == 53);
	//std::string ss = anystr.CastTo
	//BOOST_CHECK(c == 53);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE_END()