#include "engine/engine_pch.hpp"
#include "EventController.hpp"

namespace engine::events
{
	CEventController::CEventController()
	{
		for (uint8_t i = 0; i < static_cast<uint8_t>(EEventBus::_COUNT); ++i)
		{
			m_EventBuses[i].SetEventBusType((static_cast<EEventBus>(i)));
		}
	}


	void CEventController::Frame()
	{
		for (CEventBus& bus : m_EventBuses)
		{
			bus.Frame();
		}
	}

	void CEventController::SendEvent(IEvent* _event)
	{
		const EEventBus bus = _event->GetBusType();
		const event_t event_type = _event->GetEventType();
		m_EventBuses[(uint8_t)bus].SendEvent(_event);
	}
}