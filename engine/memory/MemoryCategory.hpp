#pragma once

#include "engine/rtti/RTTIMacros.hpp"
#include "engine/rtti/RTTIEnum.hpp"

namespace engine::memory
{
	enum class EMemoryCategory
	{
		Undefined,
		Array,
		Renderer,
		Filesystem,
		Events,
		Core,
		Memory,
		Game,
		Player,
		Component,
		Scene,
		Physics,
		Audio,
		Network,
		Scripting,
		Animation,
		AI,
		Tools,
		UI,
		Texture,
		Shader,
		Material,
		Mesh,
		Font,
		Text,
		Test,
		Count
	};
}
