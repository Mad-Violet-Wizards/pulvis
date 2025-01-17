#pragma once

namespace engine::events
{
	class CEventBus;
	class IEvent;

	//////////////////////////////////////////////////////////////////////////
	using event_t = size_t;	
	//////////////////////////////////////////////////////////////////////////
	enum class EEventBus : uint8_t
	{
		Invalid = UINT8_MAX,
		Engine = 0,
		Renderer,
		Window,
		Input,
		Audio,
		Physics,
		Game,
		Tests,
		_COUNT
	};
	//////////////////////////////////////////////////////////////////////////
	using EventBusTypeBindingMap = std::unordered_map<event_t, EEventBus>;
	//////////////////////////////////////////////////////////////////////////
}
