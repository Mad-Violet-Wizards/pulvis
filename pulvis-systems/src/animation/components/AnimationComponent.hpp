#pragma once

#include "animation/AnimationTypes.hpp"

namespace pulvis::systems::animation
{
	struct SAnimation;
	struct SAnimationSequence;

	struct SAnimationComponent final
	{
		const SAnimation* Animation = nullptr;
		const SAnimationSequence* Sequence = nullptr;

		anim_id_t       AnimationId = INVALID_ANIM;
		sequence_id_t   SequenceId = INVALID_SEQUENCE;
		float           Speed = 1.f;
		EAnimationFlags Flags = EAnimationFlags::None;
	};
} // namespace pulvis::systems::animation