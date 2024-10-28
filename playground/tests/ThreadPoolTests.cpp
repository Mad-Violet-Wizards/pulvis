#include "engine/engine_pch.hpp"
#include "ThreadPoolTests.hpp"

#include <functional>

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::threads;
using namespace engine::threads::tests;

TEST_CASE("Thread Function Wrapper", "[Threads]")
{
	SECTION("Simple functions wrapper")
	{
		CThreadFunctionWrapper<void> wrapper_void(MocVoid);
		wrapper_void();

		CThreadFunctionWrapper<int> wrapper(MocNoSleepReturn);
		const int result = wrapper();
		REQUIRE(result == s_MocReturnValue);

		CThreadFunctionWrapper<int, int, int> wrapper_add(MocAddTwoValues);
		const int result_add = wrapper_add(1, 2);
		REQUIRE(result_add == 3);

		int a = 1;
		int b = 2;
		int res = 0;
		CThreadFunctionWrapper<void, int&, int&, int&> wrapper_ref(RefAddTwoValues);
		wrapper_ref(a, b, res);
		REQUIRE(res == 3);
	}

	SECTION("Class method wrapper")
	{
		MocThreadNotSharedClass instance;
		CThreadFunctionWrapper<void> wrapper_void(&instance, &MocThreadNotSharedClass::MocVoid);
		wrapper_void();

		MocThreadNotSharedClass instance2;
		CThreadFunctionWrapper<int, int, int> wrapper_add(&instance2, &MocThreadNotSharedClass::MocAddTwoValues);
		const int result_add = wrapper_add(1, 2);
		REQUIRE(result_add == 3);

		int a = 1;
		int b = 2;
		int res = 0;
		CThreadFunctionWrapper<void, int&, int&, int&> wrapper_ref(&instance, &MocThreadNotSharedClass::RefAddTwoValues);
		wrapper_ref(a, b, res);
		REQUIRE(res == 3);
	}

	SECTION("Lambda wrapper")
	{
		CThreadFunctionWrapper<void> wrapper_void([]() { std::string hello = "hello world"; hello[0] = 'X'; });
		wrapper_void();

		CThreadFunctionWrapper<int, int, int> wrapper_add([](int _a, int _b) { return _a + _b; });
		const int result_add = wrapper_add(1, 2);
		REQUIRE(result_add == 3);

		int a = 1;
		int b = 2;
		int res = 0;
		CThreadFunctionWrapper<void> wrapper_ref([&a, &b, &res]() { res = a + b; });
		wrapper_ref();
		REQUIRE(res == 3);
	}

#if defined(BENCHMARKING)
	SECTION("Performance tests vs STD")
	{
		// Requirement: CThreadFunctionWrapper
		// should be at least 3-4 times faster
		// than standard library std::function.

		BENCHMARK("Thread Function Wrapper")
		{
			CThreadFunctionWrapper<int, int, int> wrapper(MocAddTwoValues);
			const int res = wrapper(100, 200);
		};

		BENCHMARK("STD simple function")
		{
			std::function<int()> stdFunction = std::bind(MocAddTwoValues, 100, 200);
			const int stdResult = stdFunction();
		};
	}
#endif
}

TEST_CASE("Thread Task", "[Threads]")
{
	SECTION("Simple functions")
	{
		CThreadTask task(MocAddTwoValues, 64, 64);
		task.Execute();
		const int result = task.GetFuture<int>().get();
		REQUIRE(result == 128);

		int a = 1;
		int b = 3;
		int c = 0;

		CThreadTask task2(RefAddTwoValues, a, b, c);
		task2.Execute();
		task2.GetFuture<void>().get();
		REQUIRE(c == 4);
	}

	SECTION("Class methods")
	{
		MocThreadNotSharedClass instance;
		CThreadTask task(&instance, &MocThreadNotSharedClass::MocAddTwoValues, 64, 64);
		task.Execute();
		const int result = task.GetFuture<int>().get();
		REQUIRE(result == 128);

		int a = 1;
		int b = 3;
		int c = 0;

		MocThreadNotSharedClass instance2;
		CThreadTask task2(&instance2, &MocThreadNotSharedClass::RefAddTwoValues, a, b, c);

		task2.Execute();
		task2.GetFuture<void>().get();
		REQUIRE(c == 4);
	}

	SECTION("Lambdas")
	{
		CThreadTask task([](int _a, int _b) { return _a + _b; }, 64, 64);
		task.Execute();
		const int result = task.GetFuture<int>().get();
		REQUIRE(result == 128);

		int a = 1;
		int b = 3;
		int c = 0;

		CThreadTask task2([&a, &b, &c]() { c = a + b; });
		task2.Execute();
		task2.GetFuture<void>().get();
		REQUIRE(c == 4);
	}
}

#if defined(HEAVY_TESTS)
TEST_CASE("Thread Pool Functional Tests", "[Threads]")
{
	SECTION("Thread Pool function task enqueued & thread pool stops.")
	{
		SThreadPoolSettings settings;
		settings.m_NumThreads = 4;
		CThreadPool threadPool(settings);

		CThreadTask task(MocSleepThread1s);
		CThreadTask task2(MocSleepThread1s);
		CThreadTask task3(MocSleepThread1s);
		CThreadTask task4(MocSleepThread1s);
		CThreadTask task5(MocSleepThread5s);

		threadPool.EnqueueTask(&task);
		threadPool.EnqueueTask(&task2);
		threadPool.EnqueueTask(&task3);
		threadPool.EnqueueTask(&task4);
		threadPool.EnqueueTask(&task5);
		threadPool.Stop();

		const int result = task.GetFuture<int>().get();
		const int result2 = task2.GetFuture<int>().get();
		const int result3 = task3.GetFuture<int>().get();
		const int result4 = task4.GetFuture<int>().get();
		const int result5 = task5.GetFuture<int>().get();

		REQUIRE(result == s_MocReturnValue);
		REQUIRE(result == s_MocReturnValue);
		REQUIRE(result == s_MocReturnValue);
		REQUIRE(result == s_MocReturnValue);
		REQUIRE(result == s_MocReturnValue);
	}

	SECTION("Thread Pool processing many tasks.")
	{
		SThreadPoolSettings settings;
		settings.m_NumThreads = 4;
		CThreadPool threadPool(settings);

		std::vector<CThreadTask> tasks;
		for (int i = 0; i < 100; ++i)
		{
			tasks.emplace_back(MocSleepThread100ms);
		}

		for (auto& task : tasks)
		{
			threadPool.EnqueueTask(&task);
		}

		for (auto& task : tasks)
		{
			const int result = task.GetFuture<int>().get();
			REQUIRE(result == s_MocReturnValue);
		}

		threadPool.Stop();
	}
}
#endif