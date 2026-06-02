#pragma once

#include "AnimationTypes.hpp"
#include "AnimationFrame.hpp"
#include "AnimationSequenceEvent.hpp"

#include <string>
#include <vector>

namespace pulvis::systems::animation
{
	struct SAnimationSequence final
	{
		std::string                          Name;
		sequence_id_t                        Id = INVALID_SEQUENCE;
		EPlaybackMode                        Playback = EPlaybackMode::Looped;
		float                                TotalDurationMs = 0.f;
		std::vector<CAnimationFrame>         Frames;
		std::vector<SAnimationSequenceEvent> Events;

		[[nodiscard]] frame_id_t FrameCount() const noexcept
		{
			return static_cast<frame_id_t>(Frames.size());
		}
	};
} // namespace pulvis::systems::animation