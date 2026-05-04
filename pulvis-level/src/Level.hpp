#pragma once

#include "Chunk.hpp"
#include "ChunkCoord.hpp"
#include "Region.hpp"

#include "RenderLayer.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cstdint>

namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::level
{
	/*
		Authored data - level designer edits these fields in the editor and
		serializes them to layers.lua. RTTI is appropriate here:
			* Inspector enumerates fields automatically
			* TableReader / future RTTI bridge can map sol::table -> struct
			* Diff/undo systems reflect over fields

		Render is *runtime-resolved* against CRenderLayerCache by name -
		not authored, not RTTI.
	*/
	struct [[Reflection::Class]] SLevelLayerDesc
	{
		[[Reflection::Field]] std::string Name;
		[[Reflection::Field]] std::string RenderLayerName;
		[[Reflection::Field]] int32_t SortKey = 0;
		[[Reflection::Field]] bool Visible = true;
		[[Reflection::Field]] bool CollisionSource = true;
		[[Reflection::Field]] bool DefaultMaterializeOnEdit = false;

		pulvis::rendering::SRenderLayerHandle Render;
	};

	/*
		Runtime container of chunks + the logical layer table.

		No RTTI: this is engine infrastructure, not authored content. The
		layer table is filled from layers.lua via CLevelService; chunks are
		filled from .chunk files by CChunkIO. Inspector / scripts touch
		SLevelLayerDesc, not the level itself.
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
		uint16_t RegisterTilesetsFromRegistry(const pulvis::fs::assets::CAssetRegistry& _registry);
		const std::string& GetTilesetPath(uint16_t _tilesetIndex) const;
		const std::unordered_map<uint16_t, std::string>& GetTilesetTable() const { return m_TilesetPaths; }

		// Layer table - level is the sole authority on layer metadata.
		uint32_t AddLayer(SLevelLayerDesc _desc);
		void SetLayer(uint32_t _layerIndex, SLevelLayerDesc _desc);
		void RemoveLastLayer();
		void ClearLayers();

		uint32_t GetLayerCount() const { return static_cast<uint32_t>(m_Layers.size()); }
		const SLevelLayerDesc* GetLayer(uint32_t _layerIndex) const;
		SLevelLayerDesc* GetLayerMutable(uint32_t _layerIndex);
		const std::vector<SLevelLayerDesc>& GetLayers() const { return m_Layers; }
		std::vector<SLevelLayerDesc>& GetLayersMutable() { return m_Layers; }

		pulvis::rendering::SRenderLayerHandle GetRenderLayerMapping(uint32_t _layerIndex) const;

	private:

		std::unordered_map<SChunkCoord, std::unique_ptr<SChunk>> m_Chunks;
		std::vector<SRegion> m_Regions;
		std::unordered_map<uint16_t, std::string> m_TilesetPaths;
		std::vector<SLevelLayerDesc> m_Layers;
	};
}