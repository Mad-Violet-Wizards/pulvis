#pragma once

#include "Event.hpp"
#include "EventHandle.hpp"

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

namespace pulvis::events
{
//////////////////////////////////////////////////////////////////////////
	struct SQueuedEvent
	{
		std::unique_ptr<IEvent> Event;
		uint64_t DispatchFrame;
		uint64_t DispatchTime;
	};
	//////////////////////////////////////////////////////////////////////////
	using script_hook_listener_t = std::function<void(const IEvent&)>;

	//////////////////////////////////////////////////////////////////////////
	class CEventScriptBridge;
	//////////////////////////////////////////////////////////////////////////
	class CEventDispatcher
	{
		public:

			CEventDispatcher();
			CEventDispatcher(const CEventDispatcher&) = delete;
			CEventDispatcher& operator=(const CEventDispatcher&) = delete;

			void Frame(float _dt);

			template<typename TEvent>
			[[nodiscard]] CEventHandle Subscribe(const std::function<void(const TEvent&)>& _callback)
			{
				const event_id_t event_id = TEvent::GetStaticEventId();
				const listener_id_t listener_id = ++m_NextListenerId;

				auto type_erased = [cb = std::move(_callback)](const IEvent& event)
				{
					cb(static_cast<const TEvent&>(event));
				};

				m_Listeners[event_id].push_back({ listener_id, std::move(type_erased) });
				return CEventHandle(this, listener_id);
			}

			template<typename TEvent, typename... Args>
			void Submit(Args&&... _args)
			{
				TEvent event(std::forward<Args>(_args)...);
				Dispatch(event);
			}

			template<typename TEventType, typename... Args>
			void EnqueueFrameDelay(uint64_t _delay_frames, Args&&... _args)
			{
				if (_delay_frames == 0)
				{
					Submit<TEventType>(std::forward<Args>(_args)...);
					return;
				}

				auto event = std::make_unique<TEventType>(std::forward<Args>(_args)...);

				SQueuedEvent queued;
				queued.Event = std::move(event);
				queued.DispatchFrame = m_CurrentFrame + _delay_frames;

				m_EventQueue.push_back(std::move(queued));
			}

			template<typename TEventType, typename... Args>
			void EnqueueTimeDelay(uint64_t _delay_ms, Args&&... _args)
			{
				if (_delay_ms == 0)
				{
					Submit<TEventType>(std::forward<Args>(_args)...);
					return;
				}

				auto event = std::make_unique<TEventType>(std::forward<Args>(_args)...);
				SQueuedEvent queued;
				queued.Event = std::move(event);

				queued.DispatchTime = m_CurrentTime + _delay_ms;
				m_EventQueue.push_back(std::move(queued));
			}

			void RegisterScriptHookListener(event_id_t _event_id, script_hook_listener_t _listener);

			void Unsubscribe(listener_id_t _listenerId);

			void ClearAll();

		private:
			
			friend class CEventScriptBridge;

			void Dispatch(const IEvent& _event);

			struct SListenerEntry
			{
				listener_id_t ListenerId;
				std::function<void(const IEvent&)> Callback;
			};

			std::unordered_map<event_id_t, script_hook_listener_t> m_ScriptHookListeners;
			std::unordered_map<event_id_t, std::vector<SListenerEntry>> m_Listeners;
			std::vector<SQueuedEvent> m_EventQueue;

			listener_id_t m_NextListenerId;
			uint64_t m_CurrentFrame;
			uint64_t m_CurrentTime;
	};
}