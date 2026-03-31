#pragma once

#include "assets/AssetPayload.hpp"

#include <cstdint>
#include <unordered_map>

namespace pulvis::fs::assets
{
	struct SGlyphInfo
	{
		float UV_X, UV_Y, UV_W, UV_H;
		float BearingX, BearingY;
		float Advance;
		uint32_t Width, Height;
	};

	struct SFontPayload : public TAssetPayload<EAssetType::Font>
	{
		uint32_t AtlasGPUHandle = 0;
		uint32_t AtlasWidth = 0;
		uint32_t AtlasHeight = 0;
		float FontSize = 0.0f;
		std::unordered_map<uint32_t, SGlyphInfo> Glyphs;
	};
}