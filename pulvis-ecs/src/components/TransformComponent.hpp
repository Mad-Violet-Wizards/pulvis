#pragma once

#include <glm/glm.hpp>

namespace pulvis::ecs
{
	struct STransformComponent final
	{
		glm::vec2 LocalPosition = { 0.f, 0.f };
		glm::vec2 LocalScale = { 1.f, 1.f };
		float     LocalRotation = 0.f;

		glm::vec2 WorldPosition = { 0.f, 0.f };
		glm::vec2 WorldScale = { 1.f, 1.f };
		float     WorldRotation = 0.f;

		bool      Dirty = true;
	};
} // namespace pulvis::ecs