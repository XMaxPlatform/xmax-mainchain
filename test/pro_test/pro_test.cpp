#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>
#include <pro/utils/singleton.hpp>

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



BOOST_AUTO_TEST_SUITE_END()