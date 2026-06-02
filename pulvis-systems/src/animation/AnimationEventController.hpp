#pragma once

#include "AnimationFrameContext.hpp"

namespace pulvis::systems::animation
{
	class IAnimationEventController
	{
	public:

		virtual ~IAnimationEventController() = default;

		virtual void OnAnimationEvent(const SAnimationFrameContext& _context) = 0;
	};
} // namespace pulvis::systems::animation