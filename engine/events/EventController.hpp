#pragma once

#include "Event.hpp"
#include "EventsEnum.hpp"
#include "EventListener.hpp"

namespace engine::events
{
	using EventListenersCollection = std::vector<IEventListener*>;

	class PULVIS_API CEventController : public Singleton<CEventController>
	{
		public:

		CEventController();
		~CEventController();
		
		friend class Singleton<CEventController>;

		void PreFrame();
		void PostFrame();

		void SubscribeEvent(EEventType _eventType, IEventListener* _listener);
		void UnsubscribeEvent(EEventType _eventType, IEventListener* _listener);

		void SendEvent(IEvent* _event);

		void ProcessEvents();

	private:

		static constexpr auto EventListenersCount = (size_t)EEventType::_COUNT;
			std::array<EventListenersCollection, EventListenersCount> m_EventListeners;

			std::queue<IEvent*> m_EventsQueue;
			std::vector<IEvent*> m_DelayedEvents;
			std::set<IEvent*> m_EventsToDelete;
		};
}