#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>
#include <pro/utils/singleton.hpp>
#include <pro/types/time.hpp>
#include <pro/types/any_value.hpp>
#include <pro/scode/shortname.hpp>

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
	const char tstring[] = "test string";
	pro::AnyValue anyi32 = (int32_t)53;
	pro::AnyValue anyi64 = (int64_t)64;
	pro::AnyValue anystr = tstring;
	pro::AnyValue anystr2 = anystr;
	int32_t c = anyi32.CastTo<int32_t>(); 
	BOOST_CHECK(c == 53);
	std::string ss = anystr2.CastTo<string>();

	BOOST_CHECK(ss == tstring);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(scode)

bool checkname(string real, pro::ShortName name)
{
	string sname = name.ToString();

	return sname == real;
}

BOOST_AUTO_TEST_CASE(scode)
{
	BOOST_CHECK(checkname("xmax", SN(xmax)));
	BOOST_CHECK(checkname("567890@", SN(567890@)));
	BOOST_CHECK(checkname("m190@xm.c", SN(m190@xm.c)));

	// check over name.
	BOOST_CHECK(checkname("m19adfgge15", SN(m19adfgge15640@xm1345678275.c)));


	BOOST_CHECK(ShortName::LegalName("xmax") == true);
	BOOST_CHECK(ShortName::LegalName("xmax$%") == false);

	BOOST_CHECK(ShortName::LegalName("m19adfgge15640@xm1345678275.c") == false);
}

BOOST_AUTO_TEST_SUITE_END()