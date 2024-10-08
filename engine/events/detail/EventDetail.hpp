#pragma once

#include "engine/events/EventTypeTraits.hpp"

//////////////////////////////////////////////////////////////////////////
namespace engine::events::detail
{
	//////////////////////////////////////////////////////////////////////////
	static EventBusTypeBindingMap s_EventBusTypeBinding;

	static void RegisterEventTypeBinding(event_t _event, EEventBus _bus)
	{
		if (s_EventBusTypeBinding.find(_event) == s_EventBusTypeBinding.end())
			s_EventBusTypeBinding.insert({ _event, _bus });
	}

	static EEventBus GetBusFromEvent(event_t _event)
	{
		auto it = s_EventBusTypeBinding.find(_event);
		if (it != s_EventBusTypeBinding.end())
		{
			return it->second;
		}
		return EEventBus::Invalid;
	}
}