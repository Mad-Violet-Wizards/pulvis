#pragma once

#include "AnimationTypes.hpp"
#include "AnimationEvent.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace pulvis::systems::animation
{
	class CAnimationEventRegistry final
	{
	public:

		CAnimationEventRegistry() = default;
		~CAnimationEventRegistry() = default;

		CAnimationEventRegistry(const CAnimationEventRegistry&) = delete;
		CAnimationEventRegistry& operator=(const CAnimationEventRegistry&) = delete;

		[[nodiscard]] bool                   Register(SAnimationEvent _event) noexcept;
		[[nodiscard]] event_id_t             FindId(std::string_view _name) const noexcept;
		[[nodiscard]] const SAnimationEvent* Find(event_id_t _id) const noexcept;
		[[nodiscard]] const std::string*		 FindName(event_id_t _id) const noexcept;
		[[nodiscard]] size_t                 Size() const noexcept { return m_Events.size(); }

		void Clear();

	private:

		std::vector<SAnimationEvent>                  m_Events;
		std::unordered_map<std::string, event_id_t>   m_NameToId;
		std::unordered_map<event_id_t, uint32_t>      m_IdToIndex;
	};
} // namespace pulvis::systems::animation