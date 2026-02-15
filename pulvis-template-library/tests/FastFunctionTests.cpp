#include "FastFunction.hpp"

#include "FastFunctionTestsMoc.hpp"
#include <catch2/catch2.hpp>
#include <functional>

using namespace pulvis::tl;
using namespace pulvis::tl::tests;

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
		FastFunction fast_function_add = FastFunction::Make<int, int, int>(Add);
		const int fast_function_add_r = fast_function_add.Invoke<int>(15, 10);
		CHECK(fast_function_add_r == 25);

		FastFunction fast_function_subtract = FastFunction::Make<int, int, int>(Subtract);
		const int fast_function_subtract_r = fast_function_subtract.Invoke<int>(15, 10);
		CHECK(fast_function_subtract_r == 5);

		FastFunction fast_function_multiply = FastFunction::Make<int, int, int>(Multiply);
		const int fast_function_multiply_r = fast_function_multiply.Invoke<int>(15, 10);
		CHECK(fast_function_multiply_r == 150);

		FastFunction fast_function_divide = FastFunction::Make<int, int, int>(Divide);
		const int fast_function_divide_r = fast_function_divide.Invoke<int>(15, 10);
		CHECK(fast_function_divide_r == 1);

		int fast_function_sum_out = 0;
		FastFunction fast_function_sum = FastFunction::Make<void, int, int, int&>(Sum);
		fast_function_sum.Invoke<void>(15, 10, fast_function_sum_out);
		CHECK(fast_function_sum_out == 25);
	}

	SECTION("Class method wrappers")
	{
		TestClass test_class;

		FastFunction fast_function_add = FastFunction(&test_class, &TestClass::Add);
		const int fast_function_add_r = fast_function_add.Invoke<int>(15, 10);
		CHECK(fast_function_add_r == 25);

		FastFunction fast_function_subtract = FastFunction(&test_class, &TestClass::Subtract);
		const int fast_function_subtract_r = fast_function_subtract.Invoke<int>(15, 10);
		CHECK(fast_function_subtract_r == 5);

		FastFunction fast_function_multiply = FastFunction(&test_class, &TestClass::Multiply);
		const int fast_function_multiply_r = fast_function_multiply.Invoke<int>(15, 10);
		CHECK(fast_function_multiply_r == 150);

		FastFunction fast_function_divide = FastFunction(&test_class, &TestClass::Divide);
		const int fast_function_divide_r = fast_function_divide.Invoke<int>(15, 10);
		CHECK(fast_function_divide_r == 1);

		int fast_function_sum_out = 0;
		FastFunction fast_function_sum = FastFunction(&test_class, &TestClass::Sum);
		fast_function_sum.Invoke<void>(15, 10, fast_function_sum_out);
		CHECK(fast_function_sum_out == 25);
	}

#ifdef RUN_BENCHMARK
	SECTION("Performance test")
	{
		BENCHMARK("Lambda wrapper (FastFunction)")
		{
			auto f = [](int a, int b) -> int { return a + b; };
			FastFunction f_fast_function = FastFunction::Make<int, int, int>(f);
			return f_fast_function.Invoke<int>(15, 10);
		};

		BENCHMARK("Function wrapper (FastFunction)")
		{
			FastFunction f_fast_function = FastFunction::Make<int, int, int>(Add);
			return f_fast_function.Invoke<int>(15, 10);
		};

		BENCHMARK("Class method wrapper (FastFunction)")
		{
			TestClass test_class;
			FastFunction f_fast_function = FastFunction(&test_class, &TestClass::Add);
			return f_fast_function.Invoke<int>(15, 10);
		};

		BENCHMARK("Lambda wrapper (std::Function")
		{
			std::function<int(int, int)> f_std_function = [](int a, int b) -> int { return a + b; };
			return f_std_function(15, 10);
		};

		BENCHMARK("Function wrapper (std::function)")
		{
			std::function<int(int, int)> f_std_function = Add;
			return f_std_function(15, 10);
		};

		BENCHMARK("Class method wrapper (std::function)")
		{
			TestClass test_class;
			std::function<int(int, int)> f_std_function = std::bind(&TestClass::Add, &test_class, std::placeholders::_1, std::placeholders::_2);
			return f_std_function(15, 10);
		};
	}
#endif
}