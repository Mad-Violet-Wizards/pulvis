#pragma once

#include "RenderLayer.hpp"

#include <glm/glm.hpp>

#include <cstdint>

namespace pulvis::ecs
{
	struct SSpriteComponent final
	{
		glm::vec2 Size = { 16.f, 16.f };
		glm::vec2 Pivot = { 0.5f, 0.5f };
		glm::vec4 UVRect = { 0.f, 0.f, 1.f, 1.f };
		glm::vec4 Color = { 1.f, 1.f, 1.f, 1.f };
		uint32_t  TextureID = 0;

		pulvis::rendering::SRenderLayerHandle Layer;
		int32_t   SortOrder = 0;
		bool      Visible = true;
	};
} // namespace pulvis::ecs