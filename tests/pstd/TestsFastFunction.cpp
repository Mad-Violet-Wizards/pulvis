#include "engine/engine_pch.hpp"

#include "tests/rtti/TestsRTTIMock.hpp"
#include "tests/threads/TestsThreadPoolMock.hpp"
#include "engine/pstd/FastFunction.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::pstd;
using namespace tests::rtti;
using namespace engine::rtti;
using namespace tests::threads;

TEST_CASE("Thread Function Wrapper", "[Threads]")
{
	SECTION("Simple functions wrapper")
	{
		FastFunction<void> wrapper_void(MocVoid);
		wrapper_void();

		FastFunction<int> wrapper(MocNoSleepReturn);
		const int result = wrapper();
		REQUIRE(result == s_MocReturnValue);

		FastFunction<int, int, int> wrapper_add(MocAddTwoValues);
		const int result_add = wrapper_add(1, 2);
		REQUIRE(result_add == 3);

		int a = 1;
		int b = 2;
		int res = 0;
		FastFunction<void, int&, int&, int&> wrapper_ref(RefAddTwoValues);
		wrapper_ref(a, b, res);
		REQUIRE(res == 3);
	}

	SECTION("Class method wrapper")
	{
		MocThreadNotSharedClass instance;
		FastFunction<void> wrapper_void(&instance, &MocThreadNotSharedClass::MocVoid);
		wrapper_void();

		MocThreadNotSharedClass instance2;
		FastFunction<int, int, int> wrapper_add(&instance2, &MocThreadNotSharedClass::MocAddTwoValues);
		const int result_add = wrapper_add(1, 2);
		REQUIRE(result_add == 3);

		int a = 1;
		int b = 2;
		int res = 0;
		FastFunction<void, int&, int&, int&> wrapper_ref(&instance, &MocThreadNotSharedClass::RefAddTwoValues);
		wrapper_ref(a, b, res);
		REQUIRE(res == 3);
	}

	SECTION("Lambda wrapper")
	{
		FastFunction<void> wrapper_void([]() { std::string hello = "hello world"; hello[0] = 'X'; });
		wrapper_void();

		FastFunction<int, int, int> wrapper_add([](int _a, int _b) { return _a + _b; });
		const int result_add = wrapper_add(1, 2);
		REQUIRE(result_add == 3);

		int a = 1;
		int b = 2;
		int res = 0;
		FastFunction<void> wrapper_ref([&a, &b, &res]() { res = a + b; });
		wrapper_ref();
		REQUIRE(res == 3);
	}
}