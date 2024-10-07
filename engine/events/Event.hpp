#pragma once

#include "detail/EventDetail.hpp"
#include "EventBus.hpp"
#include "EventTypeTraits.hpp"

//////////////////////////////////////////////////////////////////////////
#define EVENT_CLASS_API(enum_bus) \
	public: \
		static event_t GetStaticEventType() { static event_t unique_type; return reinterpret_cast<event_t>(&unique_type); } \
		static EEventBus GetStaticBusType() { return EEventBus::enum_bus; } \
		event_t GetEventType() const override { return GetStaticEventType(); } \
		EEventBus GetBusType() const override { return GetStaticBusType(); }

//////////////////////////////////////////////////////////////////////////
namespace engine::events
{
	static void RegisterEventTypeBinding(event_t _event, EEventBus _bus)
	{
		detail::RegisterEventTypeBinding(_event, _bus);
	}

	//////////////////////////////////////////////////////////////////////////
	template<class C>
	std::unique_ptr<C> CreateEvent()
	{
			return std::make_unique<C>();
	}

//////////////////////////////////////////////////////////////////////////
	enum class EProcessTime
	{
		Immediate,
		NextFrame
	};

//////////////////////////////////////////////////////////////////////////
	class IEvent
	{
	public:
		virtual ~IEvent() = default;
		virtual EProcessTime GetProcessTime() const { return EProcessTime::Immediate; }
		virtual event_t GetEventType() const = 0;
		virtual EEventBus GetBusType() const = 0;
	};
}

