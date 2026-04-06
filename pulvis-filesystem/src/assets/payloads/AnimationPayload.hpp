#pragma once

#include "assets/AssetPayload.hpp"
#include "assets/AssetEntry.hpp"

#include <cstdint>
#include <vector>
#include <string>

namespace pulvis::fs::assets
{
	struct SAnimationFrame
	{
		float UV_X, UV_Y, UV_W, UV_H;
		uint32_t DurationMs;
	};

	struct SAnimationTag
	{
		std::string Name;
		uint32_t FromFrame;
		uint32_t ToFrame;
	};

	struct SAnimationPayload : public TAssetPayload<EAssetType::Animation>
	{
		SAssetHandle TextureHandle;
		uint32_t FrameWidth = 0;
		uint32_t FrameHeight = 0;
		std::vector<SAnimationFrame> Frames;
		std::vector<SAnimationTag> Tags;
	};
}