#pragma once

#include "AnimationTypes.hpp"

namespace pulvis::systems::animation
{
	struct SAnimationSequenceEvent final
	{
		event_id_t EventId = INVALID_EVENT;
		frame_id_t Frame = 0;
	};
}