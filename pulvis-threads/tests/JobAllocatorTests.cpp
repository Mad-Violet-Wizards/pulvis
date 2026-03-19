#include "JobAllocator.hpp"

#include <catch2/catch2.hpp>

#include <set>
#include <thread>
#include <vector>

using namespace pulvis::threads;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobAllocator (basic allocation)", "[Threads][JobAllocator]")
{
	SECTION("Allocate returns non-null")
	{
		CJobAllocator<64> allocator;
		SJob* job = allocator.Allocate();
		CHECK(job != nullptr);
	}

	SECTION("Allocated job is clean")
	{
		CJobAllocator<64> allocator;
		SJob* job = allocator.Allocate();

		CHECK_FALSE(job->FastFunction);
		CHECK(job->Counter == nullptr);
		CHECK(job->Priority == EJobPriority::Normal);
	}

	SECTION("Multiple allocations return distinct pointers")
	{
		CJobAllocator<64> allocator;
		std::set<SJob*> seen;

		for (size_t i = 0; i < 64; ++i)
		{
			SJob* job = allocator.Allocate();
			CHECK(seen.find(job) == seen.end());
			seen.insert(job);
		}

		CHECK(seen.size() == 64);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobAllocator (wraparound)", "[Threads][JobAllocator]")
{
	SECTION("Allocation wraps around after PoolSize")
	{
		constexpr size_t POOL = 16;
		CJobAllocator<POOL> allocator;

		SJob* first = allocator.Allocate();
		for (size_t i = 1; i < POOL; ++i)
		{
			allocator.Allocate();
		}

		SJob* wrapped = allocator.Allocate();
		CHECK(first == wrapped);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobAllocator (Reset)", "[Threads][JobAllocator]")
{
	SECTION("Reset clears all jobs and resets index")
	{
		constexpr size_t POOL = 16;
		CJobAllocator<POOL> allocator;

		SJob* before = allocator.Allocate();
		before->Priority = EJobPriority::Critical;

		allocator.Reset();

		SJob* after = allocator.Allocate();
		CHECK(before == after);
		CHECK(after->Priority == EJobPriority::Normal);
		CHECK(after->Counter == nullptr);
		CHECK_FALSE(after->FastFunction);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobAllocator (alignment)", "[Threads][JobAllocator]")
{
	SECTION("Allocated jobs are cache-line aligned")
	{
		CJobAllocator<64> allocator;

		for (size_t i = 0; i < 64; ++i)
		{
			SJob* job = allocator.Allocate();
			CHECK(reinterpret_cast<uintptr_t>(job) % CACHE_LINE_SIZE == 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobAllocator (allocate with function)", "[Threads][JobAllocator]")
{
	SECTION("Assigned FastFunction is callable after allocation")
	{
		CJobAllocator<16> allocator;
		SJob* job = allocator.Allocate();

		int result = 0;
		job->FastFunction = pulvis::tl::FastFunction::Make<void, SJob*>(
			[&result](SJob*) { result = 42; }
		);

		CHECK(job->FastFunction);
		job->FastFunction.Invoke<void>(job);
		CHECK(result == 42);
	}
}

#ifdef RUN_BENCHMARK
//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobAllocator (performance)", "[Threads][JobAllocator][Benchmark]")
{
	SECTION("Allocation throughput")
	{
		CJobAllocator<4096> allocator;

		BENCHMARK("4096 allocations")
		{
			for (size_t i = 0; i < 4096; ++i)
			{
				allocator.Allocate();
			}
			return allocator.Allocate();
		};
	}

	SECTION("Reset throughput")
	{
		CJobAllocator<4096> allocator;
		for (size_t i = 0; i < 4096; ++i)
		{
			allocator.Allocate();
		}

		BENCHMARK("Reset 4096 pool")
		{
			allocator.Reset();
		};
	}
}
#endif