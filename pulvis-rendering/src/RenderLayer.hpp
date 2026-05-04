#pragma once

#include "RenderTarget.hpp"

#include <cstdint>
#include <string>

namespace pulvis::rendering
{
//////////////////////////////////////////////////////////////////////////
	enum class ERenderLayerBlend : uint8_t
	{
		Opaque,
		AlphaBlend,
		Additive,
		Multiply,
	};

//////////////////////////////////////////////////////////////////////////
	struct SRenderLayerDesc
	{
		std::string DebugName;
		int32_t SortKey = 0;
		ERenderLayerBlend Blend = ERenderLayerBlend::AlphaBlend;
		SRenderTargetHandle Target;
	};

//////////////////////////////////////////////////////////////////////////
	struct SRenderLayerHandle
	{
		uint32_t Id = 0;
		bool IsValid() const { return Id != 0; }
		bool operator==(const SRenderLayerHandle&) const = default;
	};
}

//////////////////////////////////////////////////////////////////////////
namespace std
{
	template<> struct hash<pulvis::rendering::SRenderLayerHandle>
	{
		size_t operator()(const pulvis::rendering::SRenderLayerHandle& _h) const noexcept
		{
			return std::hash<uint32_t>{}(_h.Id);
		}
	};
}