#include "EventDispatcher.hpp"

#include <catch2/catch2.hpp>

#include <string>
#include <vector>

using namespace pulvis::events;

//////////////////////////////////////////////////////////////////////////
namespace
{
    struct SEmptyEvent : public TEvent<SEmptyEvent>
    {
    };

    struct SIntEvent : public TEvent<SIntEvent>
    {
        int Value = 0;
        SIntEvent() = default;
        SIntEvent(int _value) : Value(_value) {}
    };

    struct SFloatEvent : public TEvent<SFloatEvent>
    {
        float X = 0.0f;
        float Y = 0.0f;
        SFloatEvent() = default;
        SFloatEvent(float _x, float _y) : X(_x), Y(_y) {}
    };

    struct SStringEvent : public TEvent<SStringEvent>
    {
        std::string Text;
        SStringEvent() = default;
        SStringEvent(const std::string& _text) : Text(_text) {}
    };
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (static event id)", "[Events][EventDispatcher]")
{
    SECTION("Different event types have different ids")
    {
			CHECK(SEmptyEvent::GetStaticEventId() != SIntEvent::GetStaticEventId());
			CHECK(SIntEvent::GetStaticEventId() != SFloatEvent::GetStaticEventId());
			CHECK(SFloatEvent::GetStaticEventId() != SStringEvent::GetStaticEventId());
    }

    SECTION("Same event type always returns same id")
    {
			CHECK(SEmptyEvent::GetStaticEventId() == SEmptyEvent::GetStaticEventId());
			CHECK(SIntEvent::GetStaticEventId() == SIntEvent::GetStaticEventId());
			CHECK(SFloatEvent::GetStaticEventId() == SFloatEvent::GetStaticEventId());
			CHECK(SStringEvent::GetStaticEventId() == SStringEvent::GetStaticEventId());
    }

    SECTION("Instance GetEventId matches static")
		{
			SIntEvent event(42);
			CHECK(event.GetEventId() == SIntEvent::GetStaticEventId());

			SFloatEvent floatEvent(3.14f, 2.71f);
			CHECK(floatEvent.GetEventId() == SFloatEvent::GetStaticEventId());
		}
}
//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (Subscribe + Submit)", "[Events][EventDispatcher]")
{
    CEventDispatcher dispatcher;


	SECTION("Single subscriber receives submitted event")
	{
		int received = 0;
		int* ptr = &received;

		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent& _event) { *ptr = _event.Value; }
		);

		dispatcher.Submit<SIntEvent>(42);

		CHECK(received == 42);
	}

	SECTION("Multiple subscribers all receive the event")
	{
		int count = 0;
		int* ptr = &count;

		CEventHandle h1 = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent&) { ++(*ptr); }
		);

		CEventHandle h2 = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent&) { ++(*ptr); }
		);

		CEventHandle h3 = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent&) { ++(*ptr); }
		);

		dispatcher.Submit<SIntEvent>(0);

		CHECK(count == 3);
	}

	SECTION("Subscriber receives correct data")
	{
		float rx = 0.0f;
		float ry = 0.0f;
		float* px = &rx;
		float* py = &ry;

		CEventHandle handle = dispatcher.Subscribe<SFloatEvent>(
			[px, py](const SFloatEvent& _event) { *px = _event.X; *py = _event.Y; }
		);

		dispatcher.Submit<SFloatEvent>(1.5f, 2.5f);

		CHECK(rx == 1.5f);
		CHECK(ry == 2.5f);
	}

	SECTION("Submit with no subscribers is a no-op")
	{
		dispatcher.Submit<SIntEvent>(123);
		CHECK(true);
	}

	SECTION("String event data preserved")
	{
		std::string received;
		std::string* ptr = &received;

		CEventHandle handle = dispatcher.Subscribe<SStringEvent>(
			[ptr](const SStringEvent& _event) { *ptr = _event.Text; }
		);

		dispatcher.Submit<SStringEvent>(std::string("hello pulvis"));

		CHECK(received == "hello pulvis");
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (multiple event types)", "[Events][EventDispatcher]")
{
	CEventDispatcher dispatcher;

	SECTION("Different types dispatched independently")
	{
		int int_received = 0;
		float float_received = 0.0f;
		int* ip = &int_received;
		float* fp = &float_received;

		CEventHandle h1 = dispatcher.Subscribe<SIntEvent>(
			[ip](const SIntEvent& _event) { *ip = _event.Value; }
		);

		CEventHandle h2 = dispatcher.Subscribe<SFloatEvent>(
			[fp](const SFloatEvent& _event) { *fp = _event.X; }
		);

		dispatcher.Submit<SIntEvent>(7);

		CHECK(int_received == 7);
		CHECK(float_received == 0.0f);

		dispatcher.Submit<SFloatEvent>(3.14f, 0.0f);

		CHECK(float_received == 3.14f);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (Unsubscribe via handle)", "[Events][EventDispatcher]")
{
	CEventDispatcher dispatcher;

	SECTION("Reset handle stops receiving events")
	{
		int received = 0;
		int* ptr = &received;

		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent& _event) { *ptr = _event.Value; }
		);

		dispatcher.Submit<SIntEvent>(10);
		CHECK(received == 10);

		handle.Reset();
		dispatcher.Submit<SIntEvent>(20);

		CHECK(received == 10);
	}

	SECTION("Handle destructor unsubscribes")
	{
		int received = 0;
		int* ptr = &received;

		{
			CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
				[ptr](const SIntEvent& _event) { *ptr = _event.Value; }
			);

			dispatcher.Submit<SIntEvent>(5);
			CHECK(received == 5);
		}

		dispatcher.Submit<SIntEvent>(99);
		CHECK(received == 5);
	}

	SECTION("Only the unsubscribed handle stops, others continue")
	{
		int a = 0;
		int b = 0;
		int* pa = &a;
		int* pb = &b;

		CEventHandle h1 = dispatcher.Subscribe<SIntEvent>(
			[pa](const SIntEvent& _event) { *pa = _event.Value; }
		);

		CEventHandle h2 = dispatcher.Subscribe<SIntEvent>(
			[pb](const SIntEvent& _event) { *pb = _event.Value; }
		);

		dispatcher.Submit<SIntEvent>(1);
		CHECK(a == 1);
		CHECK(b == 1);

		h1.Reset();
		dispatcher.Submit<SIntEvent>(2);

		CHECK(a == 1);
		CHECK(b == 2);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (handle validity)", "[Events][EventDispatcher]")
{
	CEventDispatcher dispatcher;

	SECTION("Newly created handle is valid")
	{
		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[](const SIntEvent&) {}
		);

		CHECK(handle.IsValid());
	}

	SECTION("Default handle is not valid")
	{
		CEventHandle handle;
		CHECK(!handle.IsValid());
	}

	SECTION("Reset handle is no longer valid")
	{
		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[](const SIntEvent&) {}
		);

		handle.Reset();
		CHECK(!handle.IsValid());
	}

	SECTION("Moved-from handle is not valid")
	{
		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[](const SIntEvent&) {}
		);

		CEventHandle moved = std::move(handle);
		CHECK(!handle.IsValid());
		CHECK(moved.IsValid());
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (Enqueue + Frame)", "[Events][EventDispatcher]")
{
	CEventDispatcher dispatcher;

	SECTION("Enqueued event submits after Frame")
	{
		int received = 0;
		int* ptr = &received;

		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent& _event) { *ptr = _event.Value; }
		);

		dispatcher.EnqueueFrameDelay<SIntEvent>(1, 42);
		CHECK(received == 0);

		dispatcher.Frame(0.f);
		CHECK(received == 42);
	}

	SECTION("Delayed event submits after N frames")
	{
		int received = 0;
		int* ptr = &received;

		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent& _event) { *ptr = _event.Value; }
		);

		dispatcher.EnqueueFrameDelay<SIntEvent>(3, 99);

		dispatcher.Frame(0.f);
		CHECK(received == 0);

		dispatcher.Frame(0.f);
		CHECK(received == 0);

		dispatcher.Frame(0.f);
		CHECK(received == 99);
    }
	

	SECTION("Multiple queued events submits in correct frame")
	{
		std::vector<int> results;
		std::vector<int>* rp = &results;

		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[rp](const SIntEvent& _event) { rp->push_back(_event.Value); }
		);

		dispatcher.EnqueueFrameDelay<SIntEvent>(0, 5);
		dispatcher.EnqueueFrameDelay<SIntEvent>(1, 1);
		dispatcher.EnqueueFrameDelay<SIntEvent>(2, 2);
		dispatcher.EnqueueFrameDelay<SIntEvent>(1, 3);

		CHECK(results.size() == 1);
		CHECK(results[0] == 5);

		dispatcher.Frame(0.f);
		CHECK(results.size() == 3);
		CHECK(results[1] == 1);
		CHECK(results[2] == 3);

		dispatcher.Frame(0.f);
		CHECK(results.size() == 4);
		CHECK(results[3] == 2);
	}

	SECTION("Frame with empty queue is a no-op")
	{
		dispatcher.Frame(0.f);
		CHECK(true);
	}

	SECTION("Enqueue with no subscribers does not crash")
	{
		dispatcher.EnqueueFrameDelay<SIntEvent>(0, 42);
		dispatcher.Frame(0.f);
		CHECK(true);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (ClearAll)", "[Events][EventDispatcher]")
{
	CEventDispatcher dispatcher;

	SECTION("ClearAll removes all listeners")
	{
		int received = 0;
		int* ptr = &received;

		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent& _event) { *ptr = _event.Value; }
		);

		dispatcher.ClearAll();
		dispatcher.Submit<SIntEvent>(42);

		CHECK(received == 0);
	}

	SECTION("ClearAll removes queued events")
	{
		int received = 0;
		int* ptr = &received;

		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[ptr](const SIntEvent& _event) { *ptr = _event.Value; }
		);

		dispatcher.EnqueueFrameDelay<SIntEvent>(1, 99);
		dispatcher.ClearAll();
		dispatcher.Frame(0.f);

		CHECK(received == 0);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (multiple submit calls)", "[Events][EventDispatcher]")
{
	CEventDispatcher dispatcher;

	SECTION("Each submits delivers to subscriber")
	{
		std::vector<int> results;
		std::vector<int>* rp = &results;

		CEventHandle handle = dispatcher.Subscribe<SIntEvent>(
			[rp](const SIntEvent& _event) { rp->push_back(_event.Value); }
		);

		for (int i = 0; i < 10; ++i)
		{
			dispatcher.Submit<SIntEvent>(i);
		}

		REQUIRE(results.size() == 10);
		for (int i = 0; i < 10; ++i)
		{
			CHECK(results[i] == i);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (empty event)", "[Events][EventDispatcher]")
{
	CEventDispatcher dispatcher;

	SECTION("Empty event submits and is received")
	{
		bool received = false;
		bool* ptr = &received;

		CEventHandle handle = dispatcher.Subscribe<SEmptyEvent>(
			[ptr](const SEmptyEvent&) { *ptr = true; }
		);

		dispatcher.Submit<SEmptyEvent>();

		CHECK(received);
	}
}

//////////////////////////////////////////////////////////////////////////
TEST_CASE("EventDispatcher (handle move semantics)", "[Events][EventDispatcher]")
{
	CEventDispatcher dispatcher;

	SECTION("Move-assigned handle works correctly")
	{
		int received = 0;
		int* ptr = &received;

		CEventHandle handle;

		{
			CEventHandle temp = dispatcher.Subscribe<SIntEvent>(
				[ptr](const SIntEvent& _event) { *ptr = _event.Value; }
			);

			handle = std::move(temp);
		}

		dispatcher.Submit<SIntEvent>(77);
		CHECK(received == 77);

		handle.Reset();
		dispatcher.Submit<SIntEvent>(88);
		CHECK(received == 77);
	}
}