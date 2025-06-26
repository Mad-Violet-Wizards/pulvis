#include "engine/engine_pch.hpp"
#include "tests/rtti/TestsRTTIMock.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#if defined(BENCHMARKING)
TEST_CASE("Fast function", "[PSTD]")
{
	using namespace tests::rtti;

	SECTION("Performance tests vs STD")
	{
		// Requirement: FastFunction
		// should be at least 3-4 times faster
		// than standard library std::function.

		BENCHMARK("Thread Function Wrapper")
		{
			FastFunction<int, int, int> wrapper(MocAddTwoValues);
			const int res = wrapper(100, 200);
		};

		BENCHMARK("STD simple function")
		{
			std::function<int()> stdFunction = std::bind(MocAddTwoValues, 100, 200);
			const int stdResult = stdFunction();
		};
	}
}
#endif