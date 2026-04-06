#pragma once

#include "Tile.hpp"
#include "ChunkCoord.hpp"

#include <cstdint>
#include <cstring>

namespace pulvis::level
{
	static constexpr uint32_t CHUNK_SIZE = 32;
	static constexpr uint32_t MAX_TILE_LAYERS = 8;

	/* 
		Tile layer
		 0 - Ground (grass, stone)
		 1 - Ground details (flowers, cracks)
		 2 - Objects floor (carpet, shadows, blood)
		 3 - Walls / structures
		 4 - Objects (furniture, crates, barrels)
		 5 - Overlay (roofs, treetops)
		 6 - Weather / effects
		 7 - Reserved

		 Memory:
			8 layers * 32*32 * 8B = 65,536 bytes (64KB) per chunk.
	*/

	struct SChunk
	{
		SChunkCoord Coord;
		uint32_t LayerCount = 1;
		STile Layers[MAX_TILE_LAYERS][CHUNK_SIZE * CHUNK_SIZE];

		bool IsDirty = false;

		uint64_t LastAccessTime = 0;

		STile& GetTile(uint32_t _layer, uint32_t _x, uint32_t _y)
		{
			return Layers[_layer][_y * CHUNK_SIZE + _x];
		}

		const STile& GetTile(uint32_t _layer, uint32_t _x, uint32_t _y) const
		{
			return Layers[_layer][_y * CHUNK_SIZE + _x];
		}
		void SetTile(uint32_t _layer, uint32_t _x, uint32_t _y, const STile& _tile)
		{
			Layers[_layer][_y * CHUNK_SIZE + _x] = _tile;
			IsDirty = true;
		}

		void Fill(uint32_t _layer, const STile& _tile)
		{
			for (uint32_t i = 0; i < CHUNK_SIZE * CHUNK_SIZE; ++i)
			{
				Layers[_layer][i] = _tile;
			}

			IsDirty = true;
		}

		void ClearLayer(uint32_t _layer)
		{
			std::memset(Layers[_layer], 0, sizeof(STile) * CHUNK_SIZE * CHUNK_SIZE);
			IsDirty = true;
		}

		void ClearAll()
		{
			std::memset(Layers, 0, sizeof(Layers));
			LayerCount = 1;
			IsDirty = true;
		}

		// Query helpers for AI / physics integration
		bool IsSolid(uint32_t _x, uint32_t _y) const
		{
			for (uint32_t layer = 0; layer < LayerCount; ++layer)
			{
				if (GetTile(layer, _x, _y).Flags & ETileFlag::Solid)
				{
					return true;
				}
			}
			return false;
		}

		bool IsInteractable(uint32_t _x, uint32_t _y) const
		{
			for (uint32_t layer = 0; layer < LayerCount; ++layer)
			{
				if (GetTile(layer, _x, _y).Flags & ETileFlag::Interactable)
				{
					return true;
				}
			}
			return false;
		}

		bool IsTrigger(uint32_t _x, uint32_t _y) const
		{
			for (uint32_t layer = 0; layer < LayerCount; ++layer)
			{
				if (GetTile(layer, _x, _y).Flags & ETileFlag::Trigger)
				{
					return true;
				}
			}
			return false;
		}

		bool IsOpaque(uint32_t _x, uint32_t _y) const
		{
			for (uint32_t layer = 0; layer < LayerCount; ++layer)
			{
				if (GetTile(layer, _x, _y).Flags & ETileFlag::Opaque)
				{
					return true;
				}
			}
			return false;
		}
	};
}