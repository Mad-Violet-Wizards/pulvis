#pragma once

#include "engine/rtti/RTTIMacros.hpp"
#include "engine/scriptable/Scriptable.hpp"

namespace engine::resources
{
	//////////////////////////////////////////////////////////////////////////
	enum class ETileType
	{
		Regular,
		Atlas,
		Animated,
		AtlasAnimated
	};

	//////////////////////////////////////////////////////////////////////////
	class ITile : public scriptable::IScriptable
	{
		RTTI_CLASS_API(ITile, IScriptable);

public:

			virtual ~ITile() = default;
			virtual ETileType GetTileType() const = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class CTile : public ITile
	{
		RTTI_CLASS_API(CTile, ITile);

public:

			CTile();

			static void ExportScriptable(sol::state& _lua_state);

			unsigned int m_TileWidth;
			RTTI_FIELD_API(CTile, m_TileWidth);

			unsigned int m_TileHeight;
			RTTI_FIELD_API(CTile, m_TileHeight);

			std::string m_TilePath;
			RTTI_FIELD_API(CTile, m_TilePath);

			ETileType GetTileType() const override { return ETileType::Regular; }

			bool IsValid() const override;

private:

			bool m_Valid;
	};

	//////////////////////////////////////////////////////////////////////////
	class CAtlasTile : public ITile
	{
		RTTI_CLASS_API(CAtlasTile, ITile);

public:

			CAtlasTile();

			static void ExportScriptable(sol::state& _lua_state);

			unsigned int m_TileWidth;
			RTTI_FIELD_API(CAtlasTile, m_TileWidth);

			unsigned int m_TileHeight;
			RTTI_FIELD_API(CAtlasTile, m_TileHeight);

			std::string m_TilePath;
			RTTI_FIELD_API(CAtlasTile, m_TilePath);

			unsigned int m_AtlasPositionX;
			RTTI_FIELD_API(CAtlasTile, m_AtlasPositionX);

			unsigned int m_AtlasPositionY;
			RTTI_FIELD_API(CAtlasTile, m_AtlasPositionY);

			ETileType GetTileType() const override { return ETileType::Atlas; }

			bool IsValid() const override { return false; }

private:

			bool m_Valid;
	};
}