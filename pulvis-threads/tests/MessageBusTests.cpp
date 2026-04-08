#include "MessageBus.hpp"

#include <catch2/catch2.hpp>

#ifdef _MSC_VER
#pragma comment(linker, "/STACK:4194304")
#endif

#include <atomic>
#include <string>
#include <thread>
#include <vector>

using namespace pulvis::threads;

//////////////////////////////////////////////////////////////////////////
namespace
{
	struct STestMessage
	{
		int Value;
	};

	struct STestMessage2
	{
		float X;
		float Y;
	};

	struct SLargeMessage
	{
		std::byte Data[512];
	};
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (RegisterChannel)", "[Threads][MessageBus]")
{
	SECTION("Register channel does not crash")
	{
		CMessageBus bus;
		bus.RegisterChannel(0, 4096);
		CHECK(true);
	}

	SECTION("Register multiple channels")
	{
		CMessageBus bus;
		bus.RegisterChannel(0, 4096);
		bus.RegisterChannel(1, 4096);
		bus.RegisterChannel(2, 4096);
		CHECK(true);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (Send + Drain single message)", "[Threads][MessageBus]")
{
	SECTION("Single message roundtrip")
	{
		CMessageBus bus;
		bus.RegisterChannel(0, 4096);

		int received = 0;
		bus.RegisterHandler<STestMessage>(0,
			+[](const STestMessage& _msg) {
			}
		);

		int* ptr = &received;
		bus.RegisterHandler<STestMessage>(0,
			[ptr](const STestMessage& _msg) { *ptr = _msg.Value; }
		);

		STestMessage msg{ 42 };
		bus.Send<STestMessage>(0, msg);
		bus.Drain(0);

		CHECK(received == 42);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (multiple message types)", "[Threads][MessageBus]")
{
	SECTION("Different types on same channel")
	{
		CMessageBus bus;
		bus.RegisterChannel(0, 8192);

		int int_received = 0;
		float x_received = 0.0f;
		float y_received = 0.0f;

		int* ip = &int_received;
		float* xp = &x_received;
		float* yp = &y_received;

		bus.RegisterHandler<STestMessage>(0,
			[ip](const STestMessage& _msg) { *ip = _msg.Value; }
		);

		bus.RegisterHandler<STestMessage2>(0,
			[xp, yp](const STestMessage2& _msg) { *xp = _msg.X; *yp = _msg.Y; }
		);

		STestMessage msg1{ 7 };
		STestMessage2 msg2{ 1.5f, 2.5f };

		bus.Send<STestMessage>(0, msg1);
		bus.Send<STestMessage2>(0, msg2);
		bus.Drain(0);

		CHECK(int_received == 7);
		CHECK(x_received == 1.5f);
		CHECK(y_received == 2.5f);
	}
}


//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (multiple messages same type)", "[Threads][MessageBus]")
{
	SECTION("Multiple sends drained in order")
	{
		CMessageBus bus;
		const uint32_t ch0 = bus.RegisterChannel(8192);

		std::vector<int> results;
		std::vector<int>* rp = &results;

		bus.RegisterHandler<STestMessage>(ch0,
			[rp](const STestMessage& _msg) { rp->push_back(_msg.Value); }
		);

		for (int i = 0; i < 10; ++i)
		{
			STestMessage msg{ i };
			bus.Send<STestMessage>(ch0, msg);
		}

		bus.Drain(ch0);

		REQUIRE(results.size() == 10);
		for (int i = 0; i < 10; ++i)
		{
			CHECK(results[i] == i);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (Drain on non-existent channel)", "[Threads][MessageBus]")
{
	SECTION("Drain unknown channel is no-op")
	{
		CMessageBus bus;
		bus.Drain(999);
		CHECK(true);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (Drain with no handler)", "[Threads][MessageBus]")
{
	SECTION("Messages are consumed even without matching handler")
	{
		CMessageBus bus;
		const uint32_t ch0 = bus.RegisterChannel(4096);

		STestMessage msg{ 1 };
		bus.Send<STestMessage>(ch0, msg);
		bus.Drain(ch0);

		CHECK(true);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (separate channels)", "[Threads][MessageBus]")
{
	SECTION("Messages go only to their channel")
	{
		CMessageBus bus;
		const uint32_t ch0 = bus.RegisterChannel(4096);
		const uint32_t ch1 = bus.RegisterChannel(4096);

		int ch0_val = 0;
		int ch1_val = 0;
		int* p0 = &ch0_val;
		int* p1 = &ch1_val;

		bus.RegisterHandler<STestMessage>(0,
			[p0](const STestMessage& _msg) { *p0 = _msg.Value; }
		);

		bus.RegisterHandler<STestMessage>(1,
			[p1](const STestMessage& _msg) { *p1 = _msg.Value; }
		);

		STestMessage msg0{ 10 };
		STestMessage msg1{ 20 };

		bus.Send<STestMessage>(ch0, msg0);
		bus.Send<STestMessage>(ch1, msg1);

		bus.Drain(ch0);
		CHECK(ch0_val == 10);
		CHECK(ch1_val == 0);

		bus.Drain(ch1);
		CHECK(ch1_val == 20);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (Send to non-existent channel)", "[Threads][MessageBus]")
{
	SECTION("Send to unknown channel is no-op")
	{
		CMessageBus bus;
		STestMessage msg{ 1 };
		bus.Send<STestMessage>(999, msg);
		CHECK(true);
	}
}

#ifdef RUN_BENCHMARK
//////////////////////////////////////////////////////////////////////////
TEST_CASE("MessageBus (performance)", "[Threads][MessageBus][Benchmark]")
{
	SECTION("Send throughput")
	{
		CMessageBus bus;
		const uint32_t ch0 = bus.RegisterChannel(65536);

		int dummy = 0;
		int* dp = &dummy;
		bus.RegisterHandler<STestMessage>(ch0,
			[dp](const STestMessage& _msg) { *dp += _msg.Value; }
		);

		BENCHMARK("10k Send + Drain")
		{
			for (int i = 0; i < 10000; ++i)
			{
				STestMessage msg{ i };
				bus.Send<STestMessage>(ch0, msg);
			}
			bus.Drain(ch0);
			return dummy;
		};
	}

	SECTION("Drain throughput (pre-filled)")
	{
		CMessageBus bus;
		const uint32_t ch0 = bus.RegisterChannel(65536);

		int dummy = 0;
		int* dp = &dummy;
		bus.RegisterHandler<STestMessage>(ch0,
			[dp](const STestMessage& _msg) { *dp += _msg.Value; }
		);

		for (int i = 0; i < 1000; ++i)
		{
			STestMessage msg{ i };
			bus.Send<STestMessage>(0, msg);
		}

		BENCHMARK("Drain 1000 messages")
		{
			bus.Drain(ch0);
			return dummy;
		};
	}

	SECTION("Multi-type throughput")
	{
		CMessageBus bus;
		const uint32_t ch0 = bus.RegisterChannel(65536);

		int i_dummy = 0;
		float f_dummy = 0.0f;
		int* ip = &i_dummy;
		float* fp = &f_dummy;

		bus.RegisterHandler<STestMessage>(ch0,
			[ip](const STestMessage& _msg) { *ip += _msg.Value; }
		);

		bus.RegisterHandler<STestMessage2>(ch0,
			[fp](const STestMessage2& _msg) { *fp += _msg.X; }
		);

		BENCHMARK("5k mixed Send + Drain")
		{
			for (int i = 0; i < 5000; ++i)
			{
				if (i % 2 == 0)
				{
					STestMessage msg{ i };
					bus.Send<STestMessage>(ch0, msg);
				}
				else
				{
					STestMessage2 msg{ static_cast<float>(i), 0.0f };
					bus.Send<STestMessage2>(ch0, msg);
				}
			}
			bus.Drain(ch0);
			return i_dummy;
		};
	}
}
#endif