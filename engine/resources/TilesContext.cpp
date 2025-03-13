#include "engine/engine_pch.hpp"
#include "TilesContext.hpp"
#include "TilesScriptable.hpp"

namespace engine::resources
{
	CTilesContext::CTilesContext()
	{
		m_Tiles.reserve(256);
		m_AtlasTiles.reserve(256);
	}

	void CTilesContext::Clear()
	{
	}

	void CTilesContext::LoadTile(ITile* _tile)
	{
		switch (_tile->GetTileType())
		{
		case ETileType::Regular:
		{
			m_Tiles.push_back(static_cast<CTile*>(_tile));
			break;
		}
		case ETileType::Atlas:
		{
			m_AtlasTiles.push_back(static_cast<CAtlasTile*>(_tile));
			break;
		}
		default:
		{
			PULVIS_WARNING_LOG("{}", "Unkown tile type!");
			break;
		}
		}
	}

	const std::vector<CTile*>& CTilesContext::GetTilesConstRef() const
	{
		return m_Tiles;
	}

	const std::vector<CAtlasTile*>& CTilesContext::GetAtlasTilesConstRef() const
	{
		return m_AtlasTiles;
	}
}