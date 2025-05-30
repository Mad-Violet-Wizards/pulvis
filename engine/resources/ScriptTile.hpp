#pragma once

#include "engine/rtti/RTTIMacros.hpp"
#include "engine/scriptable/Scriptable.hpp"

namespace engine::resources
{
	static constexpr unsigned int s_InvalidTileID = std::numeric_limits<unsigned int>::max();

	//////////////////////////////////////////////////////////////////////////
	enum class ETileType
	{
		Regular,
		Atlas,
		Animated,
		AtlasAnimated
	};

	//////////////////////////////////////////////////////////////////////////
	class IScriptTile : public scriptable::IScriptable
	{
		RTTI_CLASS_API(IScriptTile, IScriptable);

public:

			virtual ~IScriptTile() = default;
			virtual ETileType GetTileType() const = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class CScriptTile : public IScriptTile
	{
		RTTI_CLASS_API(CScriptTile, IScriptTile);

public:

			CScriptTile();

			static void ExportScriptable(sol::state& _lua_state);

			unsigned int m_TileWidth;
			RTTI_FIELD_API(CScriptTile, m_TileWidth);

			unsigned int m_TileHeight;
			RTTI_FIELD_API(CScriptTile, m_TileHeight);

			unsigned int m_TileID;
			RTTI_FIELD_API(CScriptTile, m_TileID);

			std::string m_TilePath;
			RTTI_FIELD_API(CScriptTile, m_TilePath);

			ETileType GetTileType() const override { return ETileType::Regular; }

			bool IsValid() const override;

private:

			bool m_Valid;
	};

	//////////////////////////////////////////////////////////////////////////
	class CScriptAtlasTile : public IScriptTile
	{
		RTTI_CLASS_API(CScriptAtlasTile, IScriptTile);

public:

			CScriptAtlasTile();

			static void ExportScriptable(sol::state& _lua_state);

			unsigned int m_TileWidth;
			RTTI_FIELD_API(CScriptAtlasTile, m_TileWidth);

			unsigned int m_TileHeight;
			RTTI_FIELD_API(CScriptAtlasTile, m_TileHeight);

			unsigned int m_TileID;
			RTTI_FIELD_API(CScriptAtlasTile, m_TileID);

			unsigned int m_AtlasPositionX;
			RTTI_FIELD_API(CScriptAtlasTile, m_AtlasPositionX);

			unsigned int m_AtlasPositionY;
			RTTI_FIELD_API(CScriptAtlasTile, m_AtlasPositionY);

			std::string m_TilePath;
			RTTI_FIELD_API(CScriptAtlasTile, m_TilePath);

			ETileType GetTileType() const override { return ETileType::Atlas; }

			bool IsValid() const override { return false; }

private:

			bool m_Valid;
	};
}