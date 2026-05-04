#pragma once

#include <cstdint>

namespace pulvis::ecs
{
	enum class ESystemPhase : uint8_t
	{
		PreUpdate,
		Script,
		Logic,
		Hierarchy,
		Trasnform,
		Physics,
		Animation,
		Render,
		PostUpdate,
		Count
	};
}