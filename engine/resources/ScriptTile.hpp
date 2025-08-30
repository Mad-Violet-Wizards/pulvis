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
	class [[Reflection::Class]] IScriptTile : public engine::scriptable::IScriptable
	{
		RTTI_CLASS_API(IScriptTile, IScriptable);

public:

			virtual ~IScriptTile() = default;
			virtual ETileType GetTileType() const = 0;
	};

	//////////////////////////////////////////////////////////////////////////
	class [[Reflection::Class]] CScriptTile : public engine::resources::IScriptTile
	{
		RTTI_CLASS_API(CScriptTile, IScriptTile);

public:

			CScriptTile();

			static void ExportScriptable(sol::state& _lua_state);

			[[Reflection::Field]]
			unsigned int m_TileWidth;

			[[Reflection::Field]]
			unsigned int m_TileHeight;

			[[Reflection::Field]]
			unsigned int m_TileID;

			[[Reflection::Field]]
			std::string m_TilePath;

			ETileType GetTileType() const override { return ETileType::Regular; }

			bool IsValid() const override;

private:

			bool m_Valid;
	};

	//////////////////////////////////////////////////////////////////////////
	class [[Reflection::Class]] CScriptAtlasTile : public engine::resources::IScriptTile
	{
		RTTI_CLASS_API(CScriptAtlasTile, IScriptTile);

public:

			CScriptAtlasTile();

			static void ExportScriptable(sol::state& _lua_state);

			[[Reflection::Field]]
			unsigned int m_TileWidth;

			[[Reflection::Field]]
			unsigned int m_TileHeight;

			[[Reflection::Field]]
			unsigned int m_TileID;

			[[Reflection::Field]]
			unsigned int m_AtlasPositionX;

			[[Reflection::Field]]
			unsigned int m_AtlasPositionY;

			[[Reflection::Field]]
			std::string m_TilePath;

			ETileType GetTileType() const override { return ETileType::Atlas; }

			bool IsValid() const override { return false; }

private:

			bool m_Valid;
	};
}