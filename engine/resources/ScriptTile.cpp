#include "engine/engine_pch.hpp"
#include "ScriptTile.hpp"
#include "ResourceService.hpp"
#include "engine/scriptable/LuaScriptableAPI.hpp"

namespace engine::resources
{
	CScriptTile::CScriptTile()
		: m_TileHeight(0)
		, m_TileWidth(0)
		, m_TileID(s_InvalidTileID)
		, m_Valid(false)
	{
	}

	void CScriptTile::ExportScriptable(sol::state& _lua_state)
	{

		const std::string_view rtti_name = rtti::CRTTITypeInfo<CScriptTile>::GetTypeName();
		const std::string_view lua_func_name = rtti::CRTTITypeInfo<CScriptTile>::GetScopedTypeName();
		const rtti::CRTTIClass* rtti_class = rtti::CRTTIClass::FindConstInStorage(rtti_name);

		_lua_state.set_function(lua_func_name, [rtti_class](sol::function tileFunc)
		{
				sol::state_view lua(tileFunc.lua_state());
				sol::table tile_table = lua.create_table();
				CScriptTile* tile = new CScriptTile();
				scriptable::LuaScriptableAPI::ExportScriptableClassFields<CScriptTile>(tile_table, rtti_class, tile);

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

	bool CScriptTile::IsValid() const
	{
		return m_TileHeight > 0 && m_TileWidth > 0 && !m_TilePath.empty();
	}

	CScriptAtlasTile::CScriptAtlasTile()
		: m_AtlasPositionX(0)
		, m_AtlasPositionY(0)
		, m_TileHeight(0)
		, m_TileWidth(0)
		, m_TileID(s_InvalidTileID)
		, m_Valid(false)
	{
	}
	void CScriptAtlasTile::ExportScriptable(sol::state& _lua_state)
	{
	}
}