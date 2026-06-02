#pragma once

#include "AnimationTypes.hpp"
#include "components/DirectionComponent.hpp"

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

namespace pulvis::systems::animation
{
	struct SAnimationActionEntry final
	{
		pulvis::ecs::EDirectionMode  Mode = pulvis::ecs::EDirectionMode::None;
		std::vector<sequence_id_t>   Map;
		std::vector<std::string>     SequenceNames;
	};

	struct SAnimationActionMap final
	{
		std::string                                            Name;
		anim_id_t                                              AnimationId = INVALID_ANIM;
		std::string                                            AnimationRefName;
		std::unordered_map<action_id_t, SAnimationActionEntry> Actions;

		[[nodiscard]] sequence_id_t Resolve(action_id_t _action,
			pulvis::ecs::direction_t _direction) const noexcept
		{
			const auto it = Actions.find(_action);
			if (it == Actions.end())
			{
				return INVALID_SEQUENCE;
			}

			const SAnimationActionEntry& entry = it->second;
			if (entry.Map.empty())
			{
				return INVALID_SEQUENCE;
			}

			const pulvis::ecs::direction_t idx =
				(entry.Mode == pulvis::ecs::EDirectionMode::Single || entry.Mode == pulvis::ecs::EDirectionMode::None)
				? pulvis::ecs::direction_t{ 0 }
			: _direction;

			return idx < entry.Map.size() ? entry.Map[idx] : INVALID_SEQUENCE;
		}
	};
} // namespace pulvis::systems::animation