#pragma once

#include "AnimationTypes.hpp"
#include "AnimationSequence.hpp"
#include "AnimationTileset.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace pulvis::systems::animation
{
	struct SAnimation final
	{
		std::string                                 Name;
		anim_id_t                                   Id = INVALID_ANIM;
		SAnimationTileset                           Tileset;
		std::vector<SAnimationSequence>             Sequences;
		std::unordered_map<sequence_id_t, uint32_t> SequenceLookup;

		[[nodiscard]] const SAnimationSequence* FindSequence(sequence_id_t _id) const noexcept
		{
			const auto it = SequenceLookup.find(_id);
			return it != SequenceLookup.end() ? &Sequences[it->second] : nullptr;
		}
	};
} // namespace pulvis::systems::animation