#include "engine/engine_pch.hpp"
#include "engine/level/Chunk.hpp"

namespace engine::level
{
	CChunk::CChunk(unsigned int _width, unsigned int _height, unsigned int _x, unsigned int _y)
		: m_Size(_width, _height)
		, m_Position(_x, _y)
		, m_Dirty(false)
	{
	}
}