#include "Level.hpp"
#include <cmath>

namespace pulvis::level
{
	SChunk* CLevel::GetChunk(const SChunkCoord& _coord)
	{
		auto it = m_Chunks.find(_coord);
		if (it == m_Chunks.end())
		{
			return nullptr;
		}

		return it->second.get();
	}

	const SChunk* CLevel::GetChunk(const SChunkCoord& _coord) const
	{
		auto it = m_Chunks.find(_coord);
		if (it == m_Chunks.end())
		{
			return nullptr;
		}
		return it->second.get();
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
		if (!chunk || _layer >= chunk->LayerCount)
		{
			return nullptr;
		}

		return &chunk->GetTile(_layer, lx, ly);
	}

	const STile* CLevel::GetTileAtWorld(float _worldX, float _worldY, float _tileSize, uint32_t _layer) const
	{
		SChunkCoord chunk_coord;
		uint32_t lx, ly;
		WorldToLocalTile(_worldX, _worldY, _tileSize, chunk_coord, lx, ly);

		const SChunk* chunk = GetChunk(chunk_coord);
		if (!chunk || _layer >= chunk->LayerCount)
		{
			return nullptr;
		}

		return &chunk->GetTile(_layer, lx, ly);
	}

	void CLevel::SetTileAtWorld(float _worldX, float _worldY, float _tileSize, uint32_t _layer, const STile& _tile)
	{
		SChunkCoord chunk_coord;
		uint32_t lx, ly;
		WorldToLocalTile(_worldX, _worldY, _tileSize, chunk_coord, lx, ly);

		SChunk* chunk = GetOrCreateChunk(chunk_coord);

		if (_layer >= chunk->LayerCount)
		{
			chunk->LayerCount = _layer + 1;
		}

		chunk->SetTile(_layer, lx, ly, _tile);
	}


	bool CLevel::IsSolidAtWorld(float _worldX, float _worldY, float _tileSize) const
	{
		SChunkCoord chunk_coord;
		uint32_t lx, ly;
		WorldToLocalTile(_worldX, _worldY, _tileSize, chunk_coord, lx, ly);

		const SChunk* chunk = GetChunk(chunk_coord);
		if (!chunk)
		{
			return false;
		}

		return chunk->IsSolid(lx, ly);
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

	const std::string& CLevel::GetTilesetPath(uint16_t _index) const
	{
		static const std::string empty;
		auto it = m_TilesetPaths.find(_index);
		if (it == m_TilesetPaths.end())
		{
			return empty;
		}
		return it->second;
	}
}