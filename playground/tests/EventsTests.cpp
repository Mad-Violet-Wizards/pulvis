#include "engine/engine_pch.hpp"
#include "EventTests.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

TEST_CASE_METHOD(engine::events::tests::EventTestsFixture, "EventTests")
{
	using namespace engine::events;
	using namespace engine::events::tests;

	SECTION("Is immediate event processed?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());
		std::unique_ptr<MocEventBool> event = CreateEvent<MocEventBool>();
		REQUIRE(event->GetProcessTime() == EProcessTime::Immediate);
		CEventController::GetInstance().SendEvent(event.get());
		REQUIRE(event->m_Handled == true);
	}

	SECTION("Is next frame event proccessed properly?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventNextFrameDelay>(GetListener());
		std::unique_ptr<MocEventNextFrameDelay> event = CreateEvent<MocEventNextFrameDelay>();
		REQUIRE(event->GetProcessTime() == EProcessTime::NextFrame);
		CEventController::GetInstance().SendEvent(event.get());
		REQUIRE(event->m_Handled == false);
		m_Application->FrameCycle();
		REQUIRE(event->m_Handled == true);
	}

	SECTION("Events on different buses?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventDifferentBus>(GetListener());
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());

		std::unique_ptr<MocEventDifferentBus> event = CreateEvent<MocEventDifferentBus>();
		REQUIRE(event->GetBusType() == EEventBus::Input);
		std::unique_ptr<MocEventBool> event2 = CreateEvent<MocEventBool>();
		CEventController::GetInstance().SendEvent(event.get());
		CEventController::GetInstance().SendEvent(event2.get());
		REQUIRE(event->m_Handled == true);
		REQUIRE(event2->m_Handled == true);
	}

	SECTION("Unsubscribed event is not proccesed?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());

		std::unique_ptr<MocEventBool> event3 = CreateEvent<MocEventBool>();
		event3->m_Handled = false;
		CEventController::GetInstance().UnsubscribeEvent<MocEventBool>(m_Listener.get());
		CEventController::GetInstance().SendEvent(event3.get());
		REQUIRE(event3->m_Handled == false);
	}

	SECTION("Two event listeners on same event?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener2());

		std::unique_ptr<MocEventBool> event4 = CreateEvent<MocEventBool>();
		CEventController::GetInstance().SendEvent(event4.get());
		REQUIRE(event4->m_Handled == true);
		REQUIRE(event4->m_Handled2 == true);
	}

	SECTION("Two event listeners but one unsubscribes?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener2());

		std::unique_ptr<MocEventBool> event4 = CreateEvent<MocEventBool>();
		CEventController::GetInstance().UnsubscribeEvent<MocEventBool>(GetListener2());
		CEventController::GetInstance().SendEvent(event4.get());
		REQUIRE(event4->m_Handled == true);
		REQUIRE(event4->m_Handled2 == false);
	}

	SECTION("Benchmark")
	{
		BENCHMARK("Many events on one bus")
		{
			CEventController::GetInstance().SubscribeEvent<MocEventInt>(GetListener());
			for (int i = 0; i < 3600; ++i)
			{
				std::unique_ptr<MocEventInt> event = CreateEvent<MocEventInt>();
				CEventController::GetInstance().SendEvent(event.get());
			}
		};
	}
}