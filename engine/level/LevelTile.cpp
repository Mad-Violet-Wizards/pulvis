#include "engine/engine_pch.hpp"
#include "LevelTile.hpp"

namespace engine::level
{
	CLevelTile::CLevelTile(unsigned int _x, unsigned int _y, unsigned int _script_tile_id)
		: m_Position(_x, _y)
		, m_ScriptTileID(_script_tile_id)
	{
	}

	CLevelTileLayer::CLevelTileLayer(unsigned int _index)
		: m_Index(_index)
	{
	}
}