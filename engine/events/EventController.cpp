#include "engine/engine_pch.hpp"
#include "EventController.hpp"

namespace engine::events
{
	CEventController::CEventController()
	{
	}

	CEventController::~CEventController()
	{
	}

	void CEventController::PreFrame()
	{
		for (auto it = m_DelayedEvents.begin(); it != m_DelayedEvents.end();)
		{
			IEvent* event = *it;
			for (IEventListener* listener : m_EventListeners[(size_t)event->GetType()])
			{
				if (listener->OnEvent(event))
				{
					if (event->m_DeleteOnUse)
					{
						it = m_DelayedEvents.erase(it);
						m_EventsToDelete.insert(event);
					}
				}
			}
		}
	}

	void CEventController::PostFrame()
	{
		for (IEvent* event : m_EventsToDelete)
			delete event;

		m_EventsToDelete.clear();
	}

	void CEventController::SubscribeEvent(EEventType _eventType, IEventListener* _listener)
	{
		m_EventListeners[(size_t)_eventType].push_back(_listener);
	}

	void CEventController::UnsubscribeEvent(EEventType _eventType, IEventListener* _listener)
	{
		auto& listeners = m_EventListeners[(size_t)_eventType];
		auto it = std::find(listeners.begin(), listeners.end(), _listener);
		if (it != listeners.end())
		{
			it = listeners.erase(it);
		}
	}

	void CEventController::SendEvent(IEvent* _event)
	{
		m_EventsQueue.push(_event);
	}

	void CEventController::ProcessEvents()
	{
		while (!m_EventsQueue.empty())
		{
			auto event = m_EventsQueue.front();
			m_EventsQueue.pop();

			for (auto listener : m_EventListeners[(size_t)event->GetType()])
			{
				if (!listener->OnEvent(event))
				{
					if (event->m_Delayed)
					{
						m_DelayedEvents.push_back(event);
					}
				}
				else
				{
					if (event->m_DeleteOnUse)
					{
						m_EventsToDelete.insert(event);
					}
				}
			}
		}
	}
}