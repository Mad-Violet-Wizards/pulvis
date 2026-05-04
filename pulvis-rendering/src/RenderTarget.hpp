#pragma once

#include <glm/glm.hpp>
#include <string>

namespace pulvis::rendering
{
	enum class ERenderTargetSizing : uint8_t
	{
		Fixed,
		MatchBackbuffer,
	};

	struct SRenderTargetDesc
	{
		std::string DebugName;
		ERenderTargetSizing Sizing = ERenderTargetSizing::MatchBackbuffer;
		glm::ivec2 SizePx = { 0, 0 };
		float Scale = 1.0f;
		bool HasDepth = false;
		bool LinearFilter = false;
	};

	struct SRenderTargetHandle
	{
		uint32_t Id = 0;
		bool IsBackbuffer() const { return Id == 0; }
		bool operator==(const SRenderTargetHandle&) const = default;
	};
}