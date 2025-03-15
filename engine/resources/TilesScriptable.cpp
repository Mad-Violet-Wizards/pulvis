#include "engine/engine_pch.hpp"
#include "TilesScriptable.hpp"
#include "ResourceService.hpp"
#include "engine/scriptable/LuaScriptableAPI.hpp"

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

		const std::string_view rtti_name = rtti::CRTTITypeInfo<CTile>::GetTypeName();
		const std::string_view lua_func_name = rtti::CRTTITypeInfo<CTile>::GetScopedTypeName();
		const rtti::CRTTIClass* rtti_class = rtti::CRTTIClass::FindConstInStorage(rtti_name);

		_lua_state.set_function(lua_func_name, [rtti_class](sol::function tileFunc)
		{
				sol::state_view lua(tileFunc.lua_state());
				sol::table tile_table = lua.create_table();
				CTile* tile = new CTile();
				scriptable::LuaScriptableAPI::ExportScriptableClassFields<CTile>(tile_table, rtti_class, tile);

				tileFunc(tile_table);

				if (tile->IsValid())
				{
					CResourceService::GetInstance().LoadTileDefinition(tile);
				}
				else
				{
					PULVIS_WARNING_LOG("{}", "Tile definition parsing failed.");
				}
		});
	}

	bool CTile::IsValid() const
	{
		return m_TileHeight > 0 && m_TileWidth > 0 && !m_TilePath.empty();
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