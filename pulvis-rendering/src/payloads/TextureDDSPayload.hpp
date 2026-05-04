#pragma once

#include "assets/AssetPayload.hpp"

namespace pulvis::rendering
{
	struct STextureDDSPayload : public pulvis::fs::assets::TAssetPayload<pulvis::fs::EAssetType::TextureDDS>
	{
		uint32_t GPUHandle = 0;
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t MipLevels = 0;
		uint32_t Format = 0; // DXGI_FORMAT
	};
}