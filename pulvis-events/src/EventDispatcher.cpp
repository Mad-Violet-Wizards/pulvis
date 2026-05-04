#include "EventDispatcher.hpp"

#include <algorithm>
#include "Logger.hpp"

namespace pulvis::events
{
	CEventDispatcher::CEventDispatcher()
		: m_CurrentTime(0)
		, m_CurrentFrame(0)
		, m_NextListenerId(0)
	{

	}

	void CEventDispatcher::Frame(float _dt)
	{
		++m_CurrentFrame;
		m_CurrentTime += static_cast<uint64_t>(_dt);

		std::vector<SQueuedEvent> to_dispatch;
		to_dispatch.reserve(m_EventQueue.size());

		auto it = m_EventQueue.begin();
		while (it != m_EventQueue.end())
		{
			bool should_dispatch = false;

			if (it->DispatchFrame > 0 && it->DispatchFrame <= m_CurrentFrame)
			{
				should_dispatch = true;
			}
			else if (it->DispatchTime > 0 && it->DispatchTime <= m_CurrentTime)
			{
				should_dispatch = true;
			}

			if (should_dispatch)
			{
				to_dispatch.push_back(std::move(*it));
				it = m_EventQueue.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (const auto& queued : to_dispatch)
		{
			if (queued.Event)
			{
				Dispatch(*queued.Event);
			}
			else
			{
				PULVIS_ERROR_LOG("CEventDispatcher::Frame - Encountered null event in queue.");
			}
		}
	}

	void CEventDispatcher::RegisterScriptHookListener(event_id_t _event_id, script_hook_listener_t _listener)
	{
		m_ScriptHookListeners[_event_id] = std::move(_listener);
	}

	void CEventDispatcher::Unsubscribe(listener_id_t _listenerId)
	{
		for (auto& [event_id, listeners] : m_Listeners)
		{
			auto it = std::remove_if(listeners.begin(), listeners.end(),
				[_listenerId](const SListenerEntry& entry)
				{
					return entry.ListenerId == _listenerId;
				});

			listeners.erase(it, listeners.end());
		}
	}

	void CEventDispatcher::ClearAll()
	{
		m_Listeners.clear();
		m_EventQueue.clear();
	}

	void CEventDispatcher::Dispatch(const IEvent& _event)
	{
		auto it = m_Listeners.find(_event.GetEventId());
		if (it == m_Listeners.end())
		{
			return;
		}

		for (const auto& entry : it->second)
		{
			entry.Callback(_event);
		}

		auto hook_it = m_ScriptHookListeners.find(_event.GetEventId());
		if (hook_it != m_ScriptHookListeners.end() && hook_it->second)
		{
			hook_it->second(_event);
		}
	}
}