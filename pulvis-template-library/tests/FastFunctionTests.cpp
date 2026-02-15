#include "FastFunction.hpp"

#include <catch2/catch2.hpp>

using namespace pulvis::tl;

TEST_CASE("Test", "[TL][FastFunction]")
{
	SECTION("Lambda wrappers")
	{
		auto f1 = []() -> int { return 25; };
		FastFunction f1_fast_function = FastFunction::Make<int>(f1);
		const int f1_fast_function_r = f1_fast_function.Invoke<int>();
		CHECK(f1_fast_function_r == 25);

		auto f2 = [](int a, int b) -> int { return a + b; };
		FastFunction f2_fast_function = FastFunction::Make<int, int, int>(f2);
		const int f2_fast_function_r = f2_fast_function.Invoke<int>(15, 10);
		CHECK(f2_fast_function_r == 25);

		int f3_capture_list_val = 15;
		auto f3 = [f3_capture_list_val](int x) -> int { return x * f3_capture_list_val; };
		FastFunction f3_fast_function = FastFunction::Make<int, int>(f3);
		const int f3_fast_function_r = f3_fast_function.Invoke<int>(10);
		CHECK(f3_fast_function_r == 150);

		auto f4 = [](int& a) -> void { a = -100; };
		FastFunction f4_fast_function = FastFunction::Make<void, int&>(f4);
		int f4_fast_function_r = 200;
		f4_fast_function.Invoke<void>(f4_fast_function_r);
		CHECK(f4_fast_function_r == -100);

		FastFunction f5_fast_function = FastFunction::Make<void, int&, int&>(
			[](int& a, int& b)
			{
				int tmp = a;
				a = b;
				b = tmp;
			});

		int f5_fast_function_r1 = 10;
		int f5_fast_function_r2 = 15;
		f5_fast_function.Invoke<void>(f5_fast_function_r1, f5_fast_function_r2);
		CHECK(f5_fast_function_r1 == 15);
		CHECK(f5_fast_function_r2 == 10);
	}

	SECTION("Function wrappers")
	{

	}

	SECTION("Class method wrappers")
	{

	}
}