#pragma once

#include "engine/level/LevelTile.hpp"

namespace engine::level
{
	static constexpr unsigned int s_ChunkWidth = 128; // [Number of tiles]
	static constexpr unsigned int s_ChunkHeight = 128; // [Number of tiles]

	class CChunk
	{
		public:

			CChunk(unsigned int _width, unsigned int _height, unsigned int _x, unsigned int _y);


			bool m_Dirty;
			std::vector<CLevelTileLayer> m_LayersBelowEntities;
			std::vector<CLevelTileLayer> m_LayersAboveEntities;
			math::Vector2<unsigned int> m_Size;
			math::Vector2<unsigned int> m_Position;
	};
}