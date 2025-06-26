#include "engine/engine_pch.hpp"

#include "TestsEventMocks.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#if defined(BENCHMARKING)
TEST_CASE("Events Benchmarking", "[Events]")
{
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
		CEventController::GetInstance().ClearListeners();
	}
}
#endif