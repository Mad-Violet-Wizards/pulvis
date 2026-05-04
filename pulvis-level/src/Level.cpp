#include "Level.hpp"

#include "assets/AssetRegistry.hpp"
#include <algorithm>
#include <cmath>

namespace pulvis::level
{
	SChunk* CLevel::GetChunk(const SChunkCoord& _coord)
	{
		auto it = m_Chunks.find(_coord);
		return it == m_Chunks.end() ? nullptr : it->second.get();
	}

	const SChunk* CLevel::GetChunk(const SChunkCoord& _coord) const
	{
		auto it = m_Chunks.find(_coord);
		return it == m_Chunks.end() ? nullptr : it->second.get();
	}

	SChunk* CLevel::GetOrCreateChunk(const SChunkCoord& _coord)
	{
		auto it = m_Chunks.find(_coord);
		if (it != m_Chunks.end())
		{
			return it->second.get();
		}

		std::unique_ptr<SChunk> chunk = std::make_unique<SChunk>();
		chunk->Coord = _coord;
		chunk->ClearAll();
		chunk->IsDirty = false;

		SChunk* ptr = chunk.get();
		m_Chunks[_coord] = std::move(chunk);
		return ptr;
	}

	bool CLevel::HasChunk(const SChunkCoord& _coord) const
	{
		return m_Chunks.find(_coord) != m_Chunks.end();
	}

	void CLevel::RemoveChunk(const SChunkCoord& _coord)
	{
		m_Chunks.erase(_coord);
	}

	SChunkCoord CLevel::WorldToChunkCoord(float _worldX, float _worldY, float _tileSize)
	{
		const float chunk_world_size = static_cast<float>(CHUNK_SIZE) * _tileSize;

		return SChunkCoord{
			static_cast<int32_t>(std::floor(_worldX / chunk_world_size)),
			static_cast<int32_t>(std::floor(_worldY / chunk_world_size))
		};
	}

	void CLevel::WorldToLocalTile(float _worldX, float _worldY, float _tileSize, SChunkCoord& _outChunk, uint32_t& _outLocalX, uint32_t& _outLocalY)
	{
		const float chunk_world_size = static_cast<float>(CHUNK_SIZE) * _tileSize;

		_outChunk = WorldToChunkCoord(_worldX, _worldY, _tileSize);

		const float local_x = _worldX - (static_cast<float>(_outChunk.X) * chunk_world_size);
		const float local_y = _worldY - (static_cast<float>(_outChunk.Y) * chunk_world_size);

		_outLocalX = static_cast<uint32_t>(local_x / _tileSize) % CHUNK_SIZE;
		_outLocalY = static_cast<uint32_t>(local_y / _tileSize) % CHUNK_SIZE;
	}

	STile* CLevel::GetTileAtWorld(float _worldX, float _worldY, float _tileSize, uint32_t _layer)
	{
		SChunkCoord chunk_coord;
		uint32_t lx, ly;
		WorldToLocalTile(_worldX, _worldY, _tileSize, chunk_coord, lx, ly);

		SChunk* chunk = GetChunk(chunk_coord);
		return chunk ? chunk->TryGetTileMutable(_layer, lx, ly) : nullptr;
	}

	const STile* CLevel::GetTileAtWorld(float _worldX, float _worldY, float _tileSize, uint32_t _layer) const
	{
		SChunkCoord chunk_coord;
		uint32_t lx, ly;
		WorldToLocalTile(_worldX, _worldY, _tileSize, chunk_coord, lx, ly);

		const SChunk* chunk = GetChunk(chunk_coord);
		return chunk ? chunk->TryGetTile(_layer, lx, ly) : nullptr;
	}

	void CLevel::SetTileAtWorld(float _worldX, float _worldY, float _tileSize, uint32_t _layer, const STile& _tile)
	{
		SChunkCoord chunk_coord;
		uint32_t lx, ly;
		WorldToLocalTile(_worldX, _worldY, _tileSize, chunk_coord, lx, ly);

		SChunk* chunk = GetOrCreateChunk(chunk_coord);
		chunk->SetTile(_layer, lx, ly, _tile);
	}

	bool CLevel::IsSolidAtWorld(float _worldX, float _worldY, float _tileSize) const
	{
		SChunkCoord chunk_coord;
		uint32_t lx, ly;
		WorldToLocalTile(_worldX, _worldY, _tileSize, chunk_coord, lx, ly);

		const SChunk* chunk = GetChunk(chunk_coord);
		return chunk ? chunk->IsSolid(lx, ly) : false;
	}

	void CLevel::AddRegion(SRegion _region)
	{
		m_Regions.push_back(std::move(_region));
	}

	const SRegion* CLevel::GetRegionAt(const SChunkCoord& _coord) const
	{
		for (const SRegion& region : m_Regions)
		{
			if (region.ContainsChunk(_coord))
			{
				return &region;
			}
		}
		return nullptr;
	}

	const std::vector<SRegion>& CLevel::GetRegions() const
	{
		return m_Regions;
	}

	std::vector<SChunkCoord> CLevel::GetDirtyChunkCoords() const
	{
		std::vector<SChunkCoord> dirty;
		for (const auto& [coord, chunk] : m_Chunks)
		{
			if (chunk->IsDirty)
			{
				dirty.push_back(coord);
			}
		}
		return dirty;
	}

	void CLevel::ClearDirtyFlags()
	{
		for (auto& [coord, chunk] : m_Chunks)
		{
			chunk->IsDirty = false;
		}
	}

	void CLevel::RegisterTileset(uint16_t _index, const std::string& _assetPath)
	{
		m_TilesetPaths[_index] = _assetPath;
	}

	uint16_t CLevel::RegisterTilesetsFromRegistry(const pulvis::fs::assets::CAssetRegistry& _registry)
	{
		const std::deque<pulvis::fs::assets::SAssetEntry>& entries = _registry.GetEntries();

		std::vector<std::string> tileset_paths;

		for (const auto& entry : entries)
		{
			if (entry.Type != pulvis::fs::EAssetType::Tileset) { continue; }
			if (entry.State != pulvis::fs::assets::EAssetState::Ready) { continue; }
			tileset_paths.push_back(entry.VirtualPath);
		}

		std::stable_sort(tileset_paths.begin(), tileset_paths.end());

		for (uint16_t i = 0; i < static_cast<uint16_t>(tileset_paths.size()); ++i)
		{
			m_TilesetPaths[i] = tileset_paths[i];
		}

		return static_cast<uint16_t>(tileset_paths.size());
	}

	const std::string& CLevel::GetTilesetPath(uint16_t _index) const
	{
		static const std::string empty;
		auto it = m_TilesetPaths.find(_index);
		return it == m_TilesetPaths.end() ? empty : it->second;
	}

	uint32_t CLevel::AddLayer(SLevelLayerDesc _desc)
	{
		const uint32_t index = static_cast<uint32_t>(m_Layers.size());
		m_Layers.push_back(std::move(_desc));
		return index;
	}

	void CLevel::SetLayer(uint32_t _layerIndex, SLevelLayerDesc _desc)
	{
		if (_layerIndex >= m_Layers.size())
		{
			m_Layers.resize(_layerIndex + 1);
		}
		m_Layers[_layerIndex] = std::move(_desc);
	}

	void CLevel::RemoveLastLayer()
	{
		if (!m_Layers.empty())
		{
			m_Layers.pop_back();
		}
	}

	void CLevel::ClearLayers()
	{
		m_Layers.clear();
	}

	const SLevelLayerDesc* CLevel::GetLayer(uint32_t _layerIndex) const
	{
		return _layerIndex >= m_Layers.size() ? nullptr : &m_Layers[_layerIndex];
	}

	SLevelLayerDesc* CLevel::GetLayerMutable(uint32_t _layerIndex)
	{
		return _layerIndex >= m_Layers.size() ? nullptr : &m_Layers[_layerIndex];
	}

	pulvis::rendering::SRenderLayerHandle CLevel::GetRenderLayerMapping(uint32_t _layerIndex) const
	{
		if (_layerIndex >= m_Layers.size())
		{
			return {};
		}
		return m_Layers[_layerIndex].Render;
	}
}