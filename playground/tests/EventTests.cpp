#include "EventTests.hpp"

namespace engine::events::tests
{
	void MocEventListener::OnEvent(const IEvent* _event)
	{
		if (_event->GetEventType() == MocEventBool::GetStaticEventType() ||
				_event->GetEventType() == MocEventInt::GetStaticEventType() ||
			  _event->GetEventType() == MocEventNextFrameDelay::GetStaticEventType() ||
		 	  _event->GetEventType() == MocEventDifferentBus::GetStaticEventType())
		{
			m_Checksum++;
		}
	}

	void MocEventListener2::OnEvent(const IEvent* _event)
	{
		if (_event->GetEventType() == MocEventBool::GetStaticEventType())
		{
			m_Checksum++;
		}
	}
}
