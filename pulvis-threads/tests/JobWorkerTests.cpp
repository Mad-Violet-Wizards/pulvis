#include "JobWorker.hpp"
#include "JobSystem.hpp"

#include <catch2/catch2.hpp>

#include <atomic>
#include <thread>

using namespace pulvis::threads;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobWorker (construction)", "[Threads][JobWorker]")
{
	SECTION("Default state is not running")
	{
		CJobWorker worker;
		CHECK_FALSE(worker.IsRunning());
		CHECK(worker.GetId() == 0);
		CHECK(worker.GetQueueSize() == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobWorker (allocate job)", "[Threads][JobWorker]")
{
	SECTION("AllocateJob returns valid pointer")
	{
		CJobWorker worker;
		SJob* job = worker.AllocateJob();
		CHECK(job != nullptr);
		CHECK_FALSE(job->FastFunction);
		CHECK(job->Counter == nullptr);
	}

	SECTION("Multiple allocations return distinct pointers")
	{
		CJobWorker worker;
		SJob* a = worker.AllocateJob();
		SJob* b = worker.AllocateJob();
		CHECK(a != b);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobWorker (submit and steal)", "[Threads][JobWorker]")
{
	SECTION("Submit adds to queue")
	{
		CJobWorker worker;
		SJob* job = worker.AllocateJob();
		job->FastFunction = pulvis::tl::FastFunction::Make<void, SJob*>(
			[](SJob*) {}
		);

		CHECK(worker.Submit(job));
		CHECK(worker.GetQueueSize() == 1);
	}

	SECTION("StealJob returns submitted job")
	{
		CJobWorker worker;
		SJob* job = worker.AllocateJob();
		job->FastFunction = pulvis::tl::FastFunction::Make<void, SJob*>(
			[](SJob*) {}
		);

		worker.Submit(job);
		SJob* stolen = worker.StealJob();
		CHECK(stolen == job);
	}

	SECTION("StealJob returns null on empty queue")
	{
		CJobWorker worker;
		SJob* stolen = worker.StealJob();
		CHECK(stolen == nullptr);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobWorker (reset allocator)", "[Threads][JobWorker]")
{
	SECTION("ResetAllocator allows reuse of same slots")
	{
		CJobWorker worker;
		SJob* first = worker.AllocateJob();
		worker.ResetAllocator();
		SJob* second = worker.AllocateJob();
		CHECK(first == second);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobWorker (move semantics)", "[Threads][JobWorker]")
{
	SECTION("Move construction transfers state")
	{
		CJobWorker original;
		SJob* job = original.AllocateJob();
		original.Submit(job);

		CJobWorker moved(std::move(original));
		CHECK(moved.GetQueueSize() == 1);
	}

	SECTION("Move assignment transfers state")
	{
		CJobWorker a;
		SJob* job = a.AllocateJob();
		a.Submit(job);

		CJobWorker b;
		b = std::move(a);
		CHECK(b.GetQueueSize() == 1);
	}
}

#ifdef RUN_BENCHMARK
//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobWorker (performance)", "[Threads][JobWorker][Benchmark]")
{
	SECTION("Submit throughput")
	{
		CJobWorker worker;

		BENCHMARK("1024 submits")
		{
			for (int i = 0; i < 1024; ++i)
			{
				SJob* job = worker.AllocateJob();
				job->FastFunction = pulvis::tl::FastFunction::Make<void, SJob*>(
					[](SJob*) {}
				);
				worker.Submit(job);
			}
			// Drain
			while (worker.StealJob()) {}
			worker.ResetAllocator();
		};
	}

	SECTION("Steal throughput")
	{
		CJobWorker worker;
		for (int i = 0; i < 1024; ++i)
		{
			SJob* job = worker.AllocateJob();
			job->FastFunction = pulvis::tl::FastFunction::Make<void, SJob*>(
				[](SJob*) {}
			);
			worker.Submit(job);
		}

		BENCHMARK("1024 steals")
		{
			SJob* stolen = nullptr;
			int count = 0;
			while ((stolen = worker.StealJob()) != nullptr)
			{
				++count;
			}
			return count;
		};
	}
}
#endif