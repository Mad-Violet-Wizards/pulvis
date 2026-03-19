#include "JobCounter.hpp"

#include <catch2/catch2.hpp>

#include <thread>
#include <vector>

using namespace pulvis::threads;

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobCounter (default construction)", "[JobCounter]")
{
	SECTION("Initial count should be zero")
	{
		CJobCounter counter;
		REQUIRE(counter.Get() == 0);
	}

	SECTION("IsReady should be true")
	{
		CJobCounter counter;
		REQUIRE(counter.IsReady());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobCounter (custom initial count)", "[JobCounter]")
{
	SECTION("Initial count should be set correctly")
	{
		CJobCounter counter(5);
		REQUIRE(counter.Get() == 5);
	}

	SECTION("IsReady should be false when count is positive")
	{
		CJobCounter counter(3);
		REQUIRE_FALSE(counter.IsReady());
	}

	SECTION("Zero initial value is ready")
	{
		CJobCounter counter(0);
		CHECK(counter.IsReady());
	}

	SECTION("Negative initial value is ready")
	{
		CJobCounter counter(-1);
		CHECK(counter.IsReady());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobCounter (Increment / Decrement)", "[Threads][JobCounter]")
{
	SECTION("Single increment")
	{
		CJobCounter counter;
		counter.Increment();
		CHECK(counter.Get() == 1);
		CHECK_FALSE(counter.IsReady());
	}

	SECTION("Multiple increments")
	{
		CJobCounter counter;
		counter.Increment(5);
		CHECK(counter.Get() == 5);
	}

	SECTION("Decrement to zero returns true")
	{
		CJobCounter counter(1);
		bool was_last = counter.Decrement();
		CHECK(was_last);
		CHECK(counter.IsReady());
	}

	SECTION("Decrement from 2 returns false")
	{
		CJobCounter counter(2);
		bool was_last = counter.Decrement();
		CHECK_FALSE(was_last);
		CHECK_FALSE(counter.IsReady());
	}

	SECTION("Increment then decrement roundtrip")
	{
		CJobCounter counter;
		counter.Increment(3);
		counter.Decrement();
		counter.Decrement();
		counter.Decrement();
		CHECK(counter.Get() == 0);
		CHECK(counter.IsReady());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobCounter (operator += / -=)", "[Threads][JobCounter]")
{
	SECTION("Operator += increments")
	{
		CJobCounter counter;
		counter += 10;
		CHECK(counter.Get() == 10);
	}

	SECTION("Operator -= decrements")
	{
		CJobCounter counter(10);
		counter -= 5;
		CHECK(counter.Get() == 5);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobCounter (concurrent increments)", "[Threads][JobCounter]")
{
	SECTION("Multiple threads incrementing")
	{
		CJobCounter counter;
		constexpr int THREADS = 8;
		constexpr int INCREMENTS_PER_THREAD = 1000;

		std::vector<std::thread> threads;
		threads.reserve(THREADS);

		for (int i = 0; i < THREADS; ++i)
		{
			threads.emplace_back([&counter]()
				{
					for (int j = 0; j < INCREMENTS_PER_THREAD; ++j)
					{
						counter.Increment();
					}
				});
		}

		for (auto& t : threads)
		{
			t.join();
		}

		CHECK(counter.Get() == THREADS * INCREMENTS_PER_THREAD);
	}
}


//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobCounter (concurrent decrements)", "[Threads][JobCounter]")
{
	SECTION("Multiple threads decrementing to zero")
	{
		constexpr int THREADS = 8;
		constexpr int DECREMENTS_PER_THREAD = 1000;
		constexpr int TOTAL = THREADS * DECREMENTS_PER_THREAD;

		CJobCounter counter(TOTAL);

		std::vector<std::thread> threads;
		threads.reserve(THREADS);

		for (int i = 0; i < THREADS; ++i)
		{
			threads.emplace_back([&counter]()
				{
					for (int j = 0; j < DECREMENTS_PER_THREAD; ++j)
					{
						counter.Decrement();
					}
				});
		}

		for (auto& t : threads)
		{
			t.join();
		}

		CHECK(counter.Get() == 0);
		CHECK(counter.IsReady());
	}
}

#ifdef RUN_BENCHMARK
//////////////////////////////////////////////////////////////////////////
TEST_CASE("JobCounter (performance)", "[Threads][JobCounter][Benchmark]")
{
	SECTION("Increment throughput")
	{
		CJobCounter counter;

		BENCHMARK("Single-thread 10k increments")
		{
			for (int i = 0; i < 10000; ++i)
			{
				counter.Increment();
			}
			return counter.Get();
		};
	}

	SECTION("Decrement throughput")
	{
		BENCHMARK("Single-thread 10k decrements")
		{
			CJobCounter counter(10000);
			for (int i = 0; i < 10000; ++i)
			{
				counter.Decrement();
			}
			return counter.Get();
		};
	}

	SECTION("Contended increment")
	{
		BENCHMARK("4 threads x 10k increments")
		{
			CJobCounter counter;
			constexpr int THREADS = 4;
			constexpr int OPS = 10000;
			std::vector<std::thread> threads;
			threads.reserve(THREADS);

			for (int i = 0; i < THREADS; ++i)
			{
				threads.emplace_back([&counter]()
					{
						for (int j = 0; j < OPS; ++j)
						{
							counter.Increment();
						}
					});
			}

			for (auto& t : threads)
			{
				t.join();
			}

			return counter.Get();
		};
	}
}
#endif
