#pragma once

#include "AnimationTypes.hpp"
#include "components/DirectionComponent.hpp"
#include "EntityHandle.hpp"

namespace pulvis::systems::animation
{
	struct SAnimationFrameContext final
	{
		pulvis::ecs::SEntityHandle Owner;
		anim_id_t                  AnimationId = INVALID_ANIM;
		sequence_id_t              SequenceId = INVALID_SEQUENCE;
		event_id_t                 EventId = INVALID_EVENT;
		frame_id_t                 FrameIndex = 0;
		pulvis::ecs::direction_t   Direction = pulvis::ecs::INVALID_DIRECTION;
	};
} // namespace pulvis::systems::animation