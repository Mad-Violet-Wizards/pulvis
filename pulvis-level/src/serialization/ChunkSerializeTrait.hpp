#pragma once

#include "Chunk.hpp"
#include "serialization/SerializeTrait.hpp"
#include "serialization/SerializationArchive.hpp"

namespace pulvis::fs::serialization
{
	template<>
	struct TSerializeTrait<pulvis::level::SChunk>
	{
		static void Serialize(CSerializationArchive& _ar, const char* _name, const pulvis::level::SChunk& _chunk)
		{
			_ar.BeginObject(_name);

			_ar.WriteInt("coord_x", _chunk.Coord.X);
			_ar.WriteInt("coord_y", _chunk.Coord.Y);
			_ar.WriteUInt("layer_count", _chunk.LayerCount);

			constexpr size_t layer_bytes = pulvis::level::CHUNK_SIZE * pulvis::level::CHUNK_SIZE * sizeof(pulvis::level::STile);

			for (uint32_t i = 0; i < _chunk.LayerCount; ++i)
			{
				_ar.WriteRaw("layer_data", _chunk.Layers[i], layer_bytes);
			}

			_ar.EndObject();
		}

		static void Deserialize(CSerializationArchive& _ar, const char* _name, pulvis::level::SChunk& _chunk)
		{
			_ar.BeginObject(_name);

			int64_t cx = 0, cy = 0;
			uint64_t layers = 0;

			_ar.ReadInt("coord_x", cx);
			_ar.ReadInt("coord_y", cy);
			_ar.ReadUInt("layer_count", layers);

			_chunk.Coord = { static_cast<int32_t>(cx), static_cast<int32_t>(cy) };
			_chunk.LayerCount = static_cast<uint32_t>(layers);
			_chunk.IsDirty = false;

			std::memset(_chunk.Layers, 0, sizeof(_chunk.Layers));

			constexpr size_t layer_bytes = pulvis::level::CHUNK_SIZE * pulvis::level::CHUNK_SIZE * sizeof(pulvis::level::STile);

			for (uint32_t i = 0; i < _chunk.LayerCount; ++i)
			{
				_ar.ReadRaw("layer_data", _chunk.Layers[i], layer_bytes);
			}

			_ar.EndObject();
		}
    };
}