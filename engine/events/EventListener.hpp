#pragma once

#include "Event.hpp"

namespace engine::events
{
	class PULVIS_API IEventListener
	{
	public:

		virtual ~IEventListener() = default;
		virtual void OnEvent(IEvent* _event) = 0;
	};

}