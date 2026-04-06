#pragma once

#include "Chunk.hpp"
#include "ChunkCoord.hpp"
#include "Region.hpp"

#include <unordered_map>
#include <vector>
#include <memory>
#include <cstdint>

namespace pulvis::level
{
	/* 
		CLevel - represents a single map. Owns sparse hashmap of chunks (loaded on demand).

		Chunk life cycle:
			- GetOrCreateChunk() - creates empty chunk (or returns existing)
			- In the future: IWorldGenerator fills empty chunks procedurally
			- SetTileAtWorld() marks chunk as dirty
			- Streaming manager saves dirty chunks before evicting

		Region layer provides metadata overlay for any system to query
		(biome, faction, resource probabilities, shader hints, AI difficulty).
	*/
	class CLevel
	{
		public:

			CLevel() = default;
			~CLevel() = default;

			CLevel(const CLevel&) = delete;
			CLevel& operator=(const CLevel&) = delete;

			CLevel(CLevel&&) = default;
			CLevel& operator=(CLevel&&) = default;

			SChunk* GetChunk(const SChunkCoord& _coord);
			const SChunk* GetChunk(const SChunkCoord& _coord) const;
			SChunk* GetOrCreateChunk(const SChunkCoord& _coord);
			bool HasChunk(const SChunkCoord& _coord) const;
			void RemoveChunk(const SChunkCoord& _coord);

			static SChunkCoord WorldToChunkCoord(float _worldX, float _worldY, float _tileSize);
			static void WorldToLocalTile(float _worldX, float _worldY, float _tileSize, SChunkCoord& _outChunk, uint32_t& _outLocalX, uint32_t& _outLocalY);

			STile* GetTileAtWorld(float _worldX, float _worldY, float _tileSize, uint32_t _layer);
			const STile* GetTileAtWorld(float _worldX, float _worldY, float _tileSize, uint32_t _layer) const;

			void SetTileAtWorld(float _worldX, float _worldY, float _tileSize, uint32_t _layer, const STile& _tile);

			bool IsSolidAtWorld(float _worldX, float _worldY, float _tileSize) const;

			void AddRegion(SRegion _region);
			const SRegion* GetRegionAt(const SChunkCoord& _coord) const;
			const std::vector<SRegion>& GetRegions() const;

			std::vector<SChunkCoord> GetDirtyChunkCoords() const;
			void ClearDirtyFlags();

			const std::unordered_map<SChunkCoord, std::unique_ptr<SChunk>>& GetLoadedChunks() const { return m_Chunks; }
			size_t GetLoadedChunkCount() const { return m_Chunks.size(); }

			void RegisterTileset(uint16_t _tilesetIndex, const std::string& _asset_path);
			const std::string& GetTilesetPath(uint16_t _tilesetIndex) const;
			const std::unordered_map<uint16_t, std::string>& GetTilesetTable() const { return m_TilesetPaths; }

	private:

		std::unordered_map<SChunkCoord, std::unique_ptr<SChunk>> m_Chunks;
		std::vector<SRegion> m_Regions;
		std::unordered_map<uint16_t, std::string> m_TilesetPaths;
	};
}