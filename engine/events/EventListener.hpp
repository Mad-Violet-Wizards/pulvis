#pragma once

#include "Event.hpp"

namespace engine::events
{
	class IEventListener
	{
	public:

		virtual ~IEventListener() = default;
		virtual void OnEvent(IEvent* _event) = 0;
	};

}