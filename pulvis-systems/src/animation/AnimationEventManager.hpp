#pragma once

#include "AnimationTypes.hpp"
#include "AnimationFrameContext.hpp"

#include <unordered_map>
#include <vector>

namespace pulvis::systems::animation
{
	class IAnimationEventController;

	class CAnimationEventManager final
	{
	public:

		CAnimationEventManager() = default;
		~CAnimationEventManager() = default;

		CAnimationEventManager(const CAnimationEventManager&) = delete;
		CAnimationEventManager& operator=(const CAnimationEventManager&) = delete;

		void RegisterController(event_id_t _event_id, IAnimationEventController* _controller);
		void UnregisterController(IAnimationEventController* _controller);

		void Dispatch(const SAnimationFrameContext& _context) const;

	private:

		std::unordered_map<event_id_t, std::vector<IAnimationEventController*>> m_Subscriptions;
	};
} // namespace pulvis::systems::animation