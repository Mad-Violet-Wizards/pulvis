#pragma once

#include "Event.hpp"
#include "EventListener.hpp"
#include "EventBus.hpp"
#include "EventTypeTraits.hpp"

namespace engine::events
{
	class PULVIS_API CEventController : public Singleton<CEventController>
	{
		public:

		CEventController();
		
		friend class Singleton<CEventController>;

		void Frame();

		template<typename C>
		void SubscribeEvent(IEventListener* _listener)
		{
			static_assert(std::is_base_of<IEvent, C>::value, "C must derive from IEvent class.");
			detail::RegisterEventTypeBinding(C::GetStaticEventType(), C::GetStaticBusType());
			const uint8_t bus_idx = (uint8_t)C::GetStaticBusType();
			m_EventBuses[bus_idx].SubscribeEvent(C::GetStaticEventType(), _listener);
		}

		template<typename C>
		void UnsubscribeEvent(IEventListener* _listener)
		{
			static_assert(std::is_base_of<IEvent, C>::value, "C must derive from IEvent class.");
			const uint8_t bus_idx = (uint8_t)C::GetStaticBusType();
			m_EventBuses[bus_idx].UnsubscribeEvent(C::GetStaticEventType(), _listener);
		}

		void SendEvent(IEvent* _event);

	private:

			static constexpr uint8_t EventBusCount = (uint8_t)EEventBus::_COUNT;
			std::array<CEventBus, EventBusCount> m_EventBuses;
		};
}