#include "engine/engine_pch.hpp"
#include "TestsThreadPoolMock.hpp"

#include "engine/threads/ThreadPool.hpp"
#include "engine/threads/ThreadTask.hpp"
#include "engine/threads/ThreadTaskHandle.hpp"
#include "engine/threads/ThreadTaskState.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::threads;
using namespace engine::pstd;
using namespace tests::threads;

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
