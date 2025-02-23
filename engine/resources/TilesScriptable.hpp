#pragma once

#include "engine/rtti/RTTIMacros.hpp"

namespace engine::resources
{
	class CTile : public engine::rtti::IRTTIBase
	{
		RTTI_CLASS_API(CTile);

		public:

			CTile() = default;

			unsigned int m_TileWidth;
			RTTI_FIELD_API(CTile, m_TileWidth);

			unsigned int m_TileHeight;
			RTTI_FIELD_API(CTile, m_TileHeight);

			std::string m_TilePath;
			RTTI_FIELD_API(CTile, m_TilePath);
	};

	class CAtlasTile : public engine::rtti::IRTTIBase
	{
		RTTI_CLASS_API(CAtlasTile);

		public:

			CAtlasTile() = default;

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
	};
}