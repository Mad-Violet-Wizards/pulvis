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
	class PULVIS_API CEventBus
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
		
		class Impl;
		Impl* m_Impl;
	};
}