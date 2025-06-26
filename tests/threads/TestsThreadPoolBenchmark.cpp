#include "engine/engine_pch.hpp"
#include <vendor/common/include/catch2/catch2.hpp>
#include "TestsThreadPoolMock.hpp"

#include "engine/threads/ThreadPool.hpp"
#include "engine/threads/ThreadTask.hpp"
#include "engine/threads/ThreadTaskHandle.hpp"
#include "engine/threads/ThreadTaskState.hpp"
#include "engine/threads/ThreadTaskScheduler.hpp"

using namespace engine::threads;
using namespace engine::pstd;
using namespace tests::threads;

#if defined(BENCHMARKING)
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