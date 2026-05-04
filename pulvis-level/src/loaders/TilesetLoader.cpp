#include "loaders/TilesetLoader.hpp"
#include "payloads/TilesetPayload.hpp"
#include "Logger.hpp"

#include <nlohmann/json.hpp>

namespace pulvis::level
{
	CTilesetLoader::CTilesetLoader(pulvis::fs::assets::CAssetRegistry& _registry)
		: m_Registry(_registry)
	{
	}

	bool CTilesetLoader::Process(pulvis::fs::assets::SAssetEntry& _entry)
	{
		if (_entry.RawData.Empty())
		{
			PULVIS_ERROR_LOG("Cannot process tileset asset '{}': Raw data is empty.", _entry.VirtualPath);
			return false;
		}

		nlohmann::json root;

		try
		{
			root = nlohmann::json::parse(
				_entry.RawData.Data(),
				_entry.RawData.Data() + _entry.RawData.Size()
			);
		}
		catch (const nlohmann::json::exception& e)
		{
			PULVIS_ERROR_LOG("JSON parse failed: {} ({})", _entry.VirtualPath, e.what());
			return false;
		}

		const std::string texture_path = root.value("texture", "");
		const uint32_t tile_w = root.value("tile_width", 0u);
		const uint32_t tile_h = root.value("tile_height", 0u);
		const uint32_t columns = root.value("columns", 0u);
		const uint32_t rows = root.value("rows", 0u);

		if (texture_path.empty() || tile_w == 0 || tile_h == 0 || columns == 0 || rows == 0)
		{
			PULVIS_ERROR_LOG("Invalid tileset descriptor: {}", _entry.VirtualPath);
			return false;
		}

		pulvis::fs::assets::SAssetHandle tex_handle = m_Registry.Find(texture_path);
		if (!tex_handle.IsValid())
		{
			tex_handle = m_Registry.RegisterAsset(_entry.Domain, texture_path, pulvis::fs::EAssetType::Texture);
		}

		const float atlas_w = static_cast<float>(columns * tile_w);
		const float atlas_h = static_cast<float>(rows * tile_h);
		const float u_step = static_cast<float>(tile_w) / atlas_w;
		const float v_step = static_cast<float>(tile_h) / atlas_h;

		const uint32_t total_tiles = columns * rows;

		std::vector<STileInfo> tiles;
		tiles.reserve(total_tiles);

		const auto& tiles_json = root.contains("tiles") ? root["tiles"] : nlohmann::json::array();

		for (uint32_t i = 0; i < total_tiles; ++i)
		{
			const uint32_t col = i % columns;
			const uint32_t row = i / columns;

			STileInfo info;
			info.UV_X = static_cast<float>(col) * u_step;
			info.UV_Y = static_cast<float>(row) * v_step;
			info.UV_W = u_step;
			info.UV_H = v_step;
			info.DefaultFlags = 0;

			if (i < tiles_json.size())
			{
				info.DefaultFlags = tiles_json[i].value("flags", static_cast<uint8_t>(0));
			}

			tiles.push_back(info);
		}

		auto payload = std::make_unique<STilesetPayload>();
		payload->TextureHandle = tex_handle;
		payload->TileWidth = tile_w;
		payload->TileHeight = tile_h;
		payload->Columns = columns;
		payload->Rows = rows;
		payload->Tiles = std::move(tiles);

		_entry.Payload = std::move(payload);
		_entry.RawData.Clear();

		return true;
	}

	bool CTilesetLoader::Unload(pulvis::fs::assets::SAssetEntry& _entry)
	{
		_entry.Payload = nullptr;
		return true;
	}
}