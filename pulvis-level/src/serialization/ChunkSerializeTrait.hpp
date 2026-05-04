#pragma once

#include "Chunk.hpp"
#include "serialization/SerializeTrait.hpp"
#include "serialization/SerializationArchive.hpp"

namespace pulvis::fs::serialization
{
	/*
		Sparse chunk serialization:
			coord_x, coord_y : int32
			layer_mask       : uint32
			For each set bit (low to high):
				layer_data   : raw blob (CHUNK_SIZE * CHUNK_SIZE * sizeof(STile))
	*/
	template<>
	struct TSerializeTrait<pulvis::level::SChunk>
	{
		static void Serialize(CSerializationArchive& _ar, const char* _name, const pulvis::level::SChunk& _chunk)
		{
			_ar.BeginObject(_name);

			_ar.WriteInt("coord_x", _chunk.Coord.X);
			_ar.WriteInt("coord_y", _chunk.Coord.Y);
			_ar.WriteUInt("layer_mask", _chunk.LayerMask);

			constexpr size_t layer_bytes = pulvis::level::CHUNK_SIZE * pulvis::level::CHUNK_SIZE * sizeof(pulvis::level::STile);

			for (uint32_t layer = 0; layer < pulvis::level::MAX_TILE_LAYERS; ++layer)
			{
				if (!_chunk.HasLayer(layer)) { continue; }

				const uint8_t slot = _chunk.LayerSlot[layer];
				const auto& data = *_chunk.Storage[slot];
				_ar.WriteRaw("layer_data", data.data(), layer_bytes);
			}

			_ar.EndObject();
		}

		static void Deserialize(CSerializationArchive& _ar, const char* _name, pulvis::level::SChunk& _chunk)
		{
			_ar.BeginObject(_name);

			int64_t cx = 0, cy = 0;
			uint64_t mask = 0;

			_ar.ReadInt("coord_x", cx);
			_ar.ReadInt("coord_y", cy);
			_ar.ReadUInt("layer_mask", mask);

			_chunk.ClearAll();
			_chunk.Coord = { static_cast<int32_t>(cx), static_cast<int32_t>(cy) };
			_chunk.IsDirty = false;

			constexpr size_t layer_bytes = pulvis::level::CHUNK_SIZE * pulvis::level::CHUNK_SIZE * sizeof(pulvis::level::STile);

			const uint32_t layer_mask = static_cast<uint32_t>(mask);

			for (uint32_t layer = 0; layer < pulvis::level::MAX_TILE_LAYERS; ++layer)
			{
				if (!(layer_mask & (1u << layer))) { continue; }

				_chunk.MaterializeLayer(layer);
				auto& data = *_chunk.Storage[_chunk.LayerSlot[layer]];
				_ar.ReadRaw("layer_data", data.data(), layer_bytes);
			}

			_chunk.IsDirty = false;
			_ar.EndObject();
		}
	};
}