#include "Job.hpp"

#include <catch2/catch2.hpp>

#include <cstdint>

using namespace pulvis::threads;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SJob (layout)", "[Threads][Job]")
{
	SECTION("Size is exactly one cache line")
	{
		CHECK(sizeof(SJob) == CACHE_LINE_SIZE);
	}

	SECTION("Size is a multiple of cache line size")
	{
		CHECK(sizeof(SJob) % CACHE_LINE_SIZE == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SJob (default state)", "[Threads][Job]")
{
	SECTION("Default FastFunction is empty")
	{
		SJob job{};
		CHECK_FALSE(job.FastFunction);
	}

	SECTION("Default Counter is null")
	{
		SJob job{};
		CHECK(job.Counter == nullptr);
	}

	SECTION("Default Priority is Normal")
	{
		SJob job{};
		CHECK(job.Priority == EJobPriority::Normal);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SJob (with FastFunction)", "[Threads][Job]")
{
	SECTION("Invoke lambda through job")
	{
		SJob job{};
		int value = 0;

		job.FastFunction = pulvis::tl::FastFunction::Make<void, SJob*>(
			[&value](SJob*) { value = 100; }
		);

		CHECK(job.FastFunction);
		job.FastFunction.Invoke<void>(&job);
		CHECK(value == 100);
	}

	SECTION("Job with counter decrement")
	{
		CJobCounter counter(1);
		SJob job{};
		job.Counter = &counter;
		job.FastFunction = pulvis::tl::FastFunction::Make<void, SJob*>(
			[](SJob*) { /* no-op */ }
		);

		job.FastFunction.Invoke<void>(&job);
		job.Counter->Decrement();

		CHECK(counter.IsReady());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("SJob (priority values)", "[Threads][Job]")
{
	SECTION("Priority enum ordering")
	{
		CHECK(static_cast<uint8_t>(EJobPriority::Low) < static_cast<uint8_t>(EJobPriority::Normal));
		CHECK(static_cast<uint8_t>(EJobPriority::Normal) < static_cast<uint8_t>(EJobPriority::High));
		CHECK(static_cast<uint8_t>(EJobPriority::High) < static_cast<uint8_t>(EJobPriority::Critical));
	}
}