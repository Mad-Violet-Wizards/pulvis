#include "engine/engine_pch.hpp"
#include "ThreadPoolTests.hpp"
#include "engine/pstd/FastFunction.hpp"

#include <functional>

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::threads;
using namespace engine::threads::tests;
using namespace engine::pstd;

#undef BENCHMARKING

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

#if defined(BENCHMARKING)
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
#endif
}

TEST_CASE("Thread Task", "[Threads]")
{
	SECTION("Simple functions")
	{
		CThreadTask task("moc_function_1", MocAddTwoValues, 64, 64);
		task.Execute();
		const int result = task.GetHandle<int>()->GetResult();
		REQUIRE(result == 128);

		int a = 1;
		int b = 3;
		int c = 0;

		CThreadTask task2("moc_function_2", RefAddTwoValues, a, b, c);
		task2.Execute();
		task2.GetHandle<void>()->GetResult();
		REQUIRE(c == 4);
	}

	SECTION("Class methods")
	{
		MocThreadNotSharedClass instance;
		CThreadTask task("moc_class_method_1", &instance, &MocThreadNotSharedClass::MocAddTwoValues, 64, 64);
		task.Execute();
		const int result = task.GetHandle<int>()->GetResult();
		REQUIRE(result == 128);

		int a = 1;
		int b = 3;
		int c = 0;

		MocThreadNotSharedClass instance2;
		CThreadTask task2("moc_class_method_2", &instance2, &MocThreadNotSharedClass::RefAddTwoValues, a, b, c);

		task2.Execute();
		task2.GetHandle<void>()->GetResult();
		REQUIRE(c == 4);
	}

	SECTION("Lambdas")
	{
		CThreadTask task("moc_lambda_1", [](int _a, int _b) { return _a + _b; }, 64, 64);
		task.Execute();
		const int result = task.GetHandle<int>()->GetResult();
		REQUIRE(result == 128);

		int a = 1;
		int b = 3;
		int c = 0;

		CThreadTask task2("moc_lambda_2", [&a, &b, &c]() { c = a + b; });
		task2.Execute();

		task2.GetHandle<void>()->GetResult();
		REQUIRE(c == 4);
	}
}

TEST_CASE("Thread Task Scheduler", "[Threads]")
{
	SECTION("Regular processing")
	{
		std::cout << "Thread Task Scheduler" << std::endl;
		SThreadPoolSettings settings;
		settings.m_NumThreads = 4;

		CThreadPool threadPool(settings);
		CThreadTaskScheduler scheduler(threadPool);

		std::vector<std::shared_ptr<CThreadTaskHandle<int>>> handles;

		auto sleep_function_100ms_1 = scheduler.ScheduleTask<int>("sleep_function_100ms_1", ETaskPriority::High, MocSleepThread100ms);
		auto sleep_function_100ms_2 = scheduler.ScheduleTask<int>("sleep_function_100ms_2", ETaskPriority::Normal, MocSleepThread100ms);
		auto sleep_function_100ms_3 = scheduler.ScheduleTask<int>("sleep_function_100ms_3", ETaskPriority::Low, MocSleepThread100ms);
		auto sleep_function_1s = scheduler.ScheduleTask<int>("sleep_function_1s", ETaskPriority::High, MocSleepThread1s);
		auto sleep_function_1s_2 = scheduler.ScheduleTask<int>("sleep_function_1s_2", ETaskPriority::Normal, MocSleepThread1s);
		auto sleep_function_5s = scheduler.ScheduleTask<int>("sleep_function_5s", ETaskPriority::High, MocSleepThread5s);

		handles.push_back(sleep_function_100ms_1);
		handles.push_back(sleep_function_100ms_2);
		handles.push_back(sleep_function_100ms_3);
		handles.push_back(sleep_function_1s);
		handles.push_back(sleep_function_1s_2);
		handles.push_back(sleep_function_5s);

		scheduler.Process();

		const int result = sleep_function_100ms_1->GetResult();
		const int result2 = sleep_function_100ms_2->GetResult();
		const int result3 = sleep_function_100ms_3->GetResult();
		const int result4 = sleep_function_1s->GetResult();
		const int result5 = sleep_function_1s_2->GetResult();
		const int result6 = sleep_function_5s->GetResult();

		REQUIRE(result == s_MocReturnValue);
		REQUIRE(result2 == s_MocReturnValue);
		REQUIRE(result3 == s_MocReturnValue);
		REQUIRE(result4 == s_MocReturnValue);
		REQUIRE(result5 == s_MocReturnValue);
		REQUIRE(result6 == s_MocReturnValue);
		std::cout << "Thread Task Scheduler finished processing." << std::endl;
	}

	SECTION("Tasks overload -- low tasks not processed.")
	{
// TODO
	}

	SECTION("Tasks overload -- low & normal tasks not processed.")
	{
// TODO
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