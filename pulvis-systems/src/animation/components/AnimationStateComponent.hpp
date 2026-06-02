#pragma once

#include "animation/AnimationTypes.hpp"

#include <cstdint>

namespace pulvis::systems::animation
{
	struct SAnimationStateComponent final
	{
		float      ElapsedMs = 0.f;
		frame_id_t CurrentFrame = 0;
		frame_id_t PrevFrame = 0;
		uint16_t   LoopCount = 0;
	};
} // namespace pulvis::systems::animation