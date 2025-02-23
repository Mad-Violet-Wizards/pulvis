#include "engine/engine_pch.hpp"
#include "TilesScriptable.hpp"

namespace engine::resources
{
	CTile::CTile()
		: m_TileHeight(0)
		, m_TileWidth(0)
		, m_Valid(false)
	{
	}

	void CTile::ExportScriptable(sol::state& _lua_state)
	{
		_lua_state.new_usertype<CTile>("CTile",
			"m_TileWidth", &CTile::m_TileWidth,
			"m_TileHeight", &CTile::m_TileHeight,
			"m_TilePath", &CTile::m_TilePath,
			"m_Valid", &CTile::m_Valid
		);
	}

	CAtlasTile::CAtlasTile()
		: m_AtlasPositionX(0)
		, m_AtlasPositionY(0)
		, m_TileHeight(0)
		, m_TileWidth(0)
		, m_Valid(false)
	{
	}
	void CAtlasTile::ExportScriptable(sol::state& _lua_state)
	{
		_lua_state.new_usertype<CAtlasTile>("CAtlasTile",
			"m_TileWidth", &CAtlasTile::m_TileWidth,
			"m_TileHeight", &CAtlasTile::m_TileHeight,
			"m_TilePath", &CAtlasTile::m_TilePath,
			"m_AtlasPositionX", &CAtlasTile::m_AtlasPositionX,
			"m_AtlasPositionY", &CAtlasTile::m_AtlasPositionY,
			"m_Valid", &CAtlasTile::m_Valid
		);
	}
}