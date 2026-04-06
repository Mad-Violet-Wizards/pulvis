#pragma once

#include "assets/AssetPayload.hpp"
#include "assets/AssetEntry.hpp"

#include <cstdint>
#include <vector>

namespace pulvis::fs::assets
{
	struct STileInfo
	{
		float UV_X, UV_Y, UV_W, UV_H;
		uint8_t DefaultFlags;
	};

	struct STilesetPayload : public TAssetPayload<EAssetType::Tileset>
	{
		SAssetHandle TextureHandle;
		uint32_t TileWidth = 0;
		uint32_t TileHeight = 0;
		uint32_t Columns = 0;
		uint32_t Rows = 0;
		std::vector<STileInfo> Tiles;
	};
}