#include "engine/engine_pch.hpp"

#include "TestsThreadPoolMock.hpp"
#include "engine/threads/ThreadPool.hpp"
#include "engine/threads/ThreadTask.hpp"
#include "engine/threads/ThreadTaskHandle.hpp"
#include "engine/threads/ThreadTaskState.hpp"
#include "engine/threads/ThreadTaskScheduler.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::threads;
using namespace engine::pstd;
using namespace tests::threads;

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

		auto sleep_function_100ms_1 = scheduler.ScheduleTask<int>("sleep_function_100ms_1", ETaskPriority::High, MocSleepThreadMiliseconds, 100);
		auto sleep_function_100ms_2 = scheduler.ScheduleTask<int>("sleep_function_100ms_2", ETaskPriority::Normal, MocSleepThreadMiliseconds, 100);
		auto sleep_function_100ms_3 = scheduler.ScheduleTask<int>("sleep_function_100ms_3", ETaskPriority::Low, MocSleepThreadMiliseconds, 100);
		auto sleep_function_1s = scheduler.ScheduleTask<int>("sleep_function_1s", ETaskPriority::High, MocSleepThreadSeconds, 1);
		auto sleep_function_1s_2 = scheduler.ScheduleTask<int>("sleep_function_1s_2", ETaskPriority::Normal, MocSleepThreadSeconds, 1);
		auto sleep_function_5s = scheduler.ScheduleTask<int>("sleep_function_5s", ETaskPriority::High, MocSleepThreadSeconds, 1);

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