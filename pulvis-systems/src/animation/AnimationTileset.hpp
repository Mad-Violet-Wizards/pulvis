#pragma once

#include <cstdint>
#include <string>
#include <glm/vec2.hpp>

namespace pulvis::systems::animation
{
	struct SAnimationTileset final
	{
		std::string TexturePath;
		uint32_t    TextureID = 0;
		glm::vec2   TextureSize = { 0.f, 0.f };
	};
} // namespace pulvis::systems::animation