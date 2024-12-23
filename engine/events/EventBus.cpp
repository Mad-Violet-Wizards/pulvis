#include "engine/engine_pch.hpp"
#include "EventBus.hpp"
#include "Event.hpp"
#include "EventListener.hpp"
#include "engine/core/Application.hpp"

namespace engine::events
{
//////////////////////////////////////////////////////////////////////////
	SNextFrameEventGuard::SNextFrameEventGuard(uint64_t _frame_number, IEvent* _event)
		: m_FrameNumber(_frame_number)
		, m_Event(_event)
	{
	}
/////////////////////////////////////////////////////////////////////////
class CEventBus::Impl
{
	public:

		std::unordered_multimap<event_t, IEventListener*> m_EventListeners;
		std::list<SNextFrameEventGuard> m_NextFrameEvents;

		EEventBus m_BusType = EEventBus::Invalid;
};
//////////////////////////////////////////////////////////////////////////
	CEventBus::CEventBus()
	{
		m_Impl = new Impl();
	}

	CEventBus::~CEventBus()
	{
		delete m_Impl;
	}

	void CEventBus::SetEventBusType(EEventBus _bus_type)
	{
		m_Impl->m_BusType = _bus_type;
	}

	EEventBus CEventBus::GetEventBusType() const
	{
		return m_Impl->m_BusType;
	}

	void CEventBus::SubscribeEvent(event_t _event, IEventListener* _listener)
	{
		m_Impl->m_EventListeners.insert({ _event, _listener });
	}

	void CEventBus::UnsubscribeEvent(event_t _event, IEventListener* _listener)
	{
		auto range = m_Impl->m_EventListeners.equal_range(_event);
		for (auto it = range.first; it != range.second;)
		{
			if (it->second == _listener)
			{
				it = m_Impl->m_EventListeners.erase(it);
				break;
			}
			else
			{
				++it;
			}
		}
	}

	void CEventBus::SendEvent(IEvent* _event)
	{
		if (_event->GetProcessTime() == EProcessTime::Immediate)
		{
			auto range = m_Impl->m_EventListeners.equal_range(_event->GetEventType());
			for (auto it = range.first; it != range.second; ++it)
			{
				it->second->OnEvent(_event);
			}
		}
		else
		{
			m_Impl->m_NextFrameEvents.emplace_back(engine::core::s_FrameContext.m_FrameNumber, _event);
		}
	}

	void CEventBus::Frame()
	{
		for (auto it = m_Impl->m_NextFrameEvents.begin(); it != m_Impl->m_NextFrameEvents.end();)
		{
			const SNextFrameEventGuard& guard = *it;

			if (engine::core::s_FrameContext.m_FrameNumber > guard.m_FrameNumber)
			{
				auto range = m_Impl->m_EventListeners.equal_range(guard.m_Event->GetEventType());
				for (auto it = range.first; it != range.second; ++it)
				{
					it->second->OnEvent(guard.m_Event);
				}

				it = m_Impl->m_NextFrameEvents.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	void CEventBus::ClearListeners()
	{
		m_Impl->m_EventListeners.clear();
	}
}