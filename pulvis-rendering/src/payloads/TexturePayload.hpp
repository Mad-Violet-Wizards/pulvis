#pragma once

#include "assets/AssetPayload.hpp"

namespace pulvis::rendering
{
	struct STexturePayload : public pulvis::fs::assets::TAssetPayload<pulvis::fs::EAssetType::Texture>
	{
		uint32_t GPUHandle = 0;
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Channels = 0;
	};
}