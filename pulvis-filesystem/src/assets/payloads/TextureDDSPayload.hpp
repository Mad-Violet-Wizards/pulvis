#pragma once

#include "assets/AssetPayload.hpp"

namespace pulvis::fs::assets
{
	struct STextureDDSPayload : public TAssetPayload<EAssetType::TextureDDS>
	{
		uint32_t GPUHandle = 0;
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t MipLevels = 0;
		uint32_t Format = 0; // DXGI_FORMAT
	};
}