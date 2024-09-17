#pragma once

#include "EventsEnum.hpp"

namespace engine::events
{
	class IEvent
	{
	public:
		virtual ~IEvent() = default;
		virtual EEventType GetType() const = 0;

		bool m_Delayed = false;
		bool m_DeleteOnUse = true;
	};
}