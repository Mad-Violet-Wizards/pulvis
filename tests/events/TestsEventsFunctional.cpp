#include "engine/engine_pch.hpp"
#include "TestsEventMocks.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

using namespace engine::events;
using namespace tests::event;

TEST_CASE_METHOD(EventTestsFixture, "[Events]")
{
	SECTION("Is immediate event processed?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());
		std::unique_ptr<MocEventBool> event = std::make_unique<MocEventBool>();
		REQUIRE(event->GetProcessTime() == EProcessTime::Immediate);
		CEventController::GetInstance().SendEvent(event.get());
		REQUIRE(GetListener()->GetChecksum() == 1);
		CEventController::GetInstance().ClearListeners();
	}

	SECTION("Is next frame event proccessed properly?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventNextFrameDelay>(GetListener());
		std::unique_ptr<MocEventNextFrameDelay> event = std::make_unique<MocEventNextFrameDelay>();
		REQUIRE(event->GetProcessTime() == EProcessTime::NextFrame);
		CEventController::GetInstance().SendEvent(event.get());
		m_Application->SimulateFrame();
		REQUIRE(GetListener()->GetChecksum() == 1);
		CEventController::GetInstance().ClearListeners();
	}

	SECTION("Events on different buses?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventDifferentBus>(GetListener());
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());

		std::unique_ptr<MocEventDifferentBus> event = std::make_unique<MocEventDifferentBus>();
		REQUIRE(event->GetBusType() == EEventBus::Input);
		std::unique_ptr<MocEventBool> event2 = std::make_unique<MocEventBool>();
		CEventController::GetInstance().SendEvent(event.get());
		CEventController::GetInstance().SendEvent(event2.get());
		REQUIRE(GetListener()->GetChecksum() == 2);
		CEventController::GetInstance().ClearListeners();
	}

	SECTION("Unsubscribed event is not proccesed?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());

		std::unique_ptr<MocEventBool> event3 = std::make_unique<MocEventBool>();
		CEventController::GetInstance().UnsubscribeEvent<MocEventBool>(GetListener());
		CEventController::GetInstance().SendEvent(event3.get());
		REQUIRE(GetListener()->GetChecksum() == 0);
		CEventController::GetInstance().ClearListeners();
	}

	SECTION("Two event listeners on same event?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener2());

		std::unique_ptr<MocEventBool> event4 = std::make_unique<MocEventBool>();
		CEventController::GetInstance().SendEvent(event4.get());
		REQUIRE(GetListener()->GetChecksum() == 1);
		REQUIRE(GetListener2()->GetChecksum() == 1);
		CEventController::GetInstance().ClearListeners();
	}

	SECTION("Two event listeners but one unsubscribes?")
	{
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener());
		CEventController::GetInstance().SubscribeEvent<MocEventBool>(GetListener2());

		std::unique_ptr<MocEventBool> event4 = std::make_unique<MocEventBool>();
		CEventController::GetInstance().UnsubscribeEvent<MocEventBool>(GetListener2());
		CEventController::GetInstance().SendEvent(event4.get());
		REQUIRE(GetListener()->GetChecksum() == 1);
		REQUIRE(GetListener2()->GetChecksum() == 0);
		CEventController::GetInstance().ClearListeners();
	}
}