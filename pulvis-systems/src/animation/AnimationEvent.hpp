#pragma once

#include "AnimationTypes.hpp"

#include <cstdint>
#include <string>

namespace pulvis::systems::animation
{
	struct [[Reflection::Class]] SAnimationEvent final
	{
		[[Reflection::Field("Serializable")]] 
		uint32_t ID = INVALID_EVENT;

		[[Reflection::Field("Serializable")]]
		std::string Name;
	};
} // namespace pulvis::systems::animation