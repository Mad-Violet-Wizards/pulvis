#include "JobSystem.hpp"

#include <catch2/catch2.hpp>

#include <atomic>
#include <thread>
#include <vector>

using namespace pulvis::threads;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobSystem (initialization)", "[Threads][JobSystem]")
{
	SECTION("Default construction is not initialized")
	{
		CJobSystem system;
		CHECK_FALSE(system.IsInitialized());
		CHECK(system.GetWorkerCount() == 0);
	}

	SECTION("Initialize creates workers")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 2;

		system.Initialize(settings);

		CHECK(system.IsInitialized());
		CHECK(system.GetWorkerCount() == 2);

		system.Shutdown();
	}

	SECTION("Double initialize is no-op")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 2;

		system.Initialize(settings);
		system.Initialize(settings);

		CHECK(system.GetWorkerCount() == 2);

		system.Shutdown();
	}

	SECTION("Shutdown resets state")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 2;

		system.Initialize(settings);
		system.Shutdown();

		CHECK_FALSE(system.IsInitialized());
		CHECK(system.GetWorkerCount() == 0);
	}

	SECTION("Double shutdown is safe")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 2;

		system.Initialize(settings);
		system.Shutdown();
		system.Shutdown();

		CHECK_FALSE(system.IsInitialized());
	}

	SECTION("Thread count clamped to hardware concurrency")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 9999;

		system.Initialize(settings);

		CHECK(system.GetWorkerCount() <= std::thread::hardware_concurrency());

		system.Shutdown();
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobSystem (single job)", "[Threads][JobSystem]")
{
	SECTION("Submit and wait on single job")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 2;
		system.Initialize(settings);

		std::atomic<int> result{ 0 };
		CJobCounter counter;

		SJob* job = system.CreateJob(
			[&result](SJob*) { result.store(42, std::memory_order_relaxed); },
			&counter
		);

		system.Submit(job);
		system.Wait(&counter);

		CHECK(result.load(std::memory_order_relaxed) == 42);

		system.Shutdown();
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobSystem (batch submit)", "[Threads][JobSystem]")
{
	SECTION("Submit batch of jobs and wait")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 4;
		system.Initialize(settings);

		constexpr size_t JOB_COUNT = 64;
		std::atomic<int> sum{ 0 };
		CJobCounter counter;

		std::vector<SJob*> jobs;
		jobs.reserve(JOB_COUNT);

		for (size_t i = 0; i < JOB_COUNT; ++i)
		{
			SJob* job = system.CreateJob(
				[&sum](SJob*) { sum.fetch_add(1, std::memory_order_relaxed); },
				&counter
			);
			jobs.push_back(job);
		}

		system.SubmitBatch(jobs.data(), jobs.size());
		system.Wait(&counter);

		CHECK(sum.load(std::memory_order_relaxed) == JOB_COUNT);

		system.Shutdown();
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobSystem (parallel correctness)", "[Threads][JobSystem]")
{
	SECTION("All jobs complete with correct data")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 4;
		system.Initialize(settings);

		constexpr size_t JOB_COUNT = 256;
		std::atomic<int64_t> accumulator{ 0 };
		CJobCounter counter;

		for (size_t i = 0; i < JOB_COUNT; ++i)
		{
			SJob* job = system.CreateJob(
				[&accumulator](SJob*) { accumulator.fetch_add(1, std::memory_order_relaxed); },
				&counter
			);
			system.Submit(job);
		}

		system.Wait(&counter);

		CHECK(accumulator.load(std::memory_order_relaxed) == JOB_COUNT);

		system.Shutdown();
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobSystem (ResetAllocators)", "[Threads][JobSystem]")
{
	SECTION("Reset allocators does not crash")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 2;
		system.Initialize(settings);

		CJobCounter counter;
		SJob* job = system.CreateJob(
			[](SJob*) {},
			&counter
		);
		system.Submit(job);
		system.Wait(&counter);

		system.ResetAllocators();

		system.Shutdown();
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobSystem (HelpWithWork from main thread)", "[Threads][JobSystem]")
{
	SECTION("Main thread helps complete jobs")
	{
		CJobSystem system;
		SJobSystemSettings settings;
		settings.ThreadsCount = 2;
		system.Initialize(settings);

		std::atomic<int> value{ 0 };
		CJobCounter counter;

		for (int i = 0; i < 16; ++i)
		{
			SJob* job = system.CreateJob(
				[&value](SJob*) { value.fetch_add(1, std::memory_order_relaxed); },
				&counter
			);
			system.Submit(job);
		}

		system.Wait(&counter);

		CHECK(value.load(std::memory_order_relaxed) == 16);

		system.Shutdown();
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobSystem (destructor calls Shutdown)", "[Threads][JobSystem]")
{
	SECTION("Destructor is safe after Initialize")
	{
		{
			CJobSystem system;
			SJobSystemSettings settings;
			settings.ThreadsCount = 2;
			system.Initialize(settings);
		}

		CHECK(true);
	}
}

#ifdef RUN_BENCHMARK
//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobSystem (performance)", "[Threads][JobSystem][Benchmark]")
{
	CJobSystem system;
	SJobSystemSettings settings;
	settings.ThreadsCount = 4;
	system.Initialize(settings);

	SECTION("Throughput: submit + wait 1024 jobs")
	{
		BENCHMARK("1024 empty jobs")
		{
			std::atomic<int> dummy{ 0 };
			CJobCounter counter;

			for (int i = 0; i < 1024; ++i)
			{
				SJob* job = system.CreateJob(
					[&dummy](SJob*) { dummy.fetch_add(1, std::memory_order_relaxed); },
					&counter
				);
				system.Submit(job);
			}

			system.Wait(&counter);
			system.ResetAllocators();
			return dummy.load();
		};
	}

	SECTION("Throughput: submit batch 1024 jobs")
	{
		BENCHMARK("1024 batch jobs")
		{
			std::atomic<int> dummy{ 0 };
			CJobCounter counter;
			std::vector<SJob*> jobs;
			jobs.reserve(1024);

			for (int i = 0; i < 1024; ++i)
			{
				jobs.push_back(system.CreateJob(
					[&dummy](SJob*) { dummy.fetch_add(1, std::memory_order_relaxed); },
					&counter
				));
			}

			system.SubmitBatch(jobs.data(), jobs.size());
			system.Wait(&counter);
			system.ResetAllocators();
			return dummy.load();
		};
	}

	SECTION("Latency: single job roundtrip")
	{
		BENCHMARK("Single job submit + wait")
		{
			CJobCounter counter;
			SJob* job = system.CreateJob(
				[](SJob*) {},
				&counter
			);
			system.Submit(job);
			system.Wait(&counter);
			system.ResetAllocators();
		};
	}

	system.Shutdown();
}
#endif