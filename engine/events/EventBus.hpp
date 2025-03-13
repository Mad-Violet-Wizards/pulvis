#pragma once

#include "engine/events/EventTypeTraits.hpp"

namespace engine::events
{
	class IEventListener;
	class IEvent;
//////////////////////////////////////////////////////////////////////////
	struct SNextFrameEventGuard
	{
		SNextFrameEventGuard(uint64_t _frame_number, IEvent* _event);

		uint64_t m_FrameNumber;
		IEvent* m_Event;
	};

//////////////////////////////////////////////////////////////////////////
	class CEventBus
	{
		public:

			CEventBus();
			~CEventBus();

			void SetEventBusType(EEventBus _bus_type);
			EEventBus GetEventBusType() const;

			void SubscribeEvent(event_t _event, IEventListener* _listener);
			void UnsubscribeEvent(event_t _event, IEventListener* _listener);

			void SendEvent(IEvent* _event);

			void Frame();
			void ClearListeners();

	private:
		
		std::unordered_multimap<event_t, IEventListener*> m_EventListeners;
		std::list<SNextFrameEventGuard> m_NextFrameEvents;

		EEventBus m_BusType = EEventBus::Invalid;
	};
}