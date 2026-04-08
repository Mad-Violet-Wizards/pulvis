#pragma once

#include "Hash.hpp"

#include <cstdint>

namespace pulvis::events
{
//////////////////////////////////////////////////////////////////////////
	using event_id_t = uint32_t;

//////////////////////////////////////////////////////////////////////////
	class IEvent
	{
		public:

			virtual ~IEvent() = default;
			virtual event_id_t GetEventId() const = 0;
			virtual const char* GetEventName() const = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	class TEvent : public IEvent
	{
		public:

			static constexpr event_id_t GetStaticEventId()
			{
				return pulvis::tl::hash::fnv1a<event_id_t>(__FUNCSIG__);
			}

			event_id_t GetEventId() const override
			{
				return GetStaticEventId();
			}

			const char* GetEventName() const override
			{
				return typeid(T).name();
			}
	};
}