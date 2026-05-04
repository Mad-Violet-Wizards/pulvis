#pragma once

#include "Tile.hpp"
#include "ChunkCoord.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace pulvis::level
{
	static constexpr uint32_t CHUNK_SIZE = 32;
	static constexpr uint32_t MAX_TILE_LAYERS = 16;

	/*
		SChunk - sparse, layer-aware storage of tiles.

		The chunk does not own layer semantics - that lives on CLevel
		(SLevelLayerDesc). The chunk only stores which layers are physically
		materialized (LayerMask) and where their data lives (LayerSlot[]).

		Memory footprint:
			* Empty chunk: ~64 B (header + empty vector)
			* Per-layer block: 32 * 32 * 8 B = 8 KB
			* Each chunk independently chooses its layer subset.
	*/

	using SChunkLayer = std::array<STile, CHUNK_SIZE* CHUNK_SIZE>;

	struct SChunk
	{
		SChunkCoord Coord;
		uint32_t LayerMask = 0;
		uint8_t LayerSlot[MAX_TILE_LAYERS]
		{
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
		};
		std::vector<std::unique_ptr<SChunkLayer>> Storage;

		bool IsDirty = false;
		uint64_t LastAccessTime = 0;

		SChunk() = default;
		~SChunk() = default;

		SChunk(const SChunk&) = delete;
		SChunk& operator=(const SChunk&) = delete;

		SChunk(SChunk&&) = default;
		SChunk& operator=(SChunk&&) = default;

		bool HasLayer(uint32_t _layer) const
		{
			return _layer < MAX_TILE_LAYERS && (LayerMask & (1u << _layer)) != 0;
		}

		uint32_t GetMaterializedLayerCount() const
		{
			uint32_t count = 0;
			for (uint32_t i = 0; i < MAX_TILE_LAYERS; ++i)
			{
				if (LayerMask & (1u << i)) { ++count; }
			}
			return count;
		}

		const STile* TryGetTile(uint32_t _layer, uint32_t _x, uint32_t _y) const
		{
			if (!HasLayer(_layer)) { return nullptr; }
			return &(*Storage[LayerSlot[_layer]])[_y * CHUNK_SIZE + _x];
		}

		STile* TryGetTileMutable(uint32_t _layer, uint32_t _x, uint32_t _y)
		{
			if (!HasLayer(_layer)) { return nullptr; }
			return &(*Storage[LayerSlot[_layer]])[_y * CHUNK_SIZE + _x];
		}

		const STile& GetTile(uint32_t _layer, uint32_t _x, uint32_t _y) const
		{
			static const STile s_Empty{};
			const STile* tile = TryGetTile(_layer, _x, _y);
			return tile ? *tile : s_Empty;
		}

		STile& GetOrCreateTile(uint32_t _layer, uint32_t _x, uint32_t _y)
		{
			MaterializeLayer(_layer);
			return (*Storage[LayerSlot[_layer]])[_y * CHUNK_SIZE + _x];
		}

		void SetTile(uint32_t _layer, uint32_t _x, uint32_t _y, const STile& _tile)
		{
			MaterializeLayer(_layer);
			(*Storage[LayerSlot[_layer]])[_y * CHUNK_SIZE + _x] = _tile;
			IsDirty = true;
		}

		void Fill(uint32_t _layer, const STile& _tile)
		{
			MaterializeLayer(_layer);
			SChunkLayer& data = *Storage[LayerSlot[_layer]];
			for (uint32_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; ++i)
			{
				data[i] = _tile;
			}
			IsDirty = true;
		}

		void ClearLayer(uint32_t _layer)
		{
			if (!HasLayer(_layer)) { return; }

			const uint8_t slot = LayerSlot[_layer];

			Storage.erase(Storage.begin() + slot);
			LayerSlot[_layer] = 0xFF;
			LayerMask &= ~(1u << _layer);

			for (uint32_t i = 0; i < MAX_TILE_LAYERS; ++i)
			{
				if (LayerSlot[i] != 0xFF && LayerSlot[i] > slot)
				{
					LayerSlot[i] = static_cast<uint8_t>(LayerSlot[i] - 1);
				}
			}

			IsDirty = true;
		}

		void ClearAll()
		{
			Storage.clear();
			LayerMask = 0;
			for (uint32_t i = 0; i < MAX_TILE_LAYERS; ++i)
			{
				LayerSlot[i] = 0xFF;
			}
			IsDirty = true;
		}

		void MaterializeLayer(uint32_t _layer)
		{
			if (_layer >= MAX_TILE_LAYERS || HasLayer(_layer))
			{
				return;
			}

			LayerSlot[_layer] = static_cast<uint8_t>(Storage.size());
			Storage.emplace_back(std::make_unique<SChunkLayer>());
			std::memset(Storage.back()->data(), 0, sizeof(STile) * CHUNK_SIZE * CHUNK_SIZE);
			LayerMask |= (1u << _layer);
		}

		bool IsSolid(uint32_t _x, uint32_t _y) const { return AnyLayerHasFlag(_x, _y, ETileFlag::Solid); }
		bool IsInteractable(uint32_t _x, uint32_t _y) const { return AnyLayerHasFlag(_x, _y, ETileFlag::Interactable); }
		bool IsTrigger(uint32_t _x, uint32_t _y) const { return AnyLayerHasFlag(_x, _y, ETileFlag::Trigger); }
		bool IsOpaque(uint32_t _x, uint32_t _y) const { return AnyLayerHasFlag(_x, _y, ETileFlag::Opaque); }

	private:

		bool AnyLayerHasFlag(uint32_t _x, uint32_t _y, ETileFlag _tile_flag) const
		{
			uint32_t mask = LayerMask;
			for (uint32_t layer = 0; layer < MAX_TILE_LAYERS && mask; ++layer)
			{
				const uint32_t bit = 1u << layer;
				if (!(mask & bit)) { continue; }
				mask &= ~bit;

				if ((*Storage[LayerSlot[layer]])[_y * CHUNK_SIZE + _x].Flags & static_cast<uint32_t>(_tile_flag))
				{
					return true;
				}
			}
			return false;
		}
	};
}