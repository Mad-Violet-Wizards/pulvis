#include "engine/engine_pch.hpp"
#include "TilesContext.hpp"
#include "TilesScriptable.hpp"

namespace engine::resources
{
	CTilesContext::CTilesContext()
	{
		m_TilesDefinitions.reserve(256);
		m_AtlasTilesDefinitions.reserve(256);
	}

	CTilesContext::~CTilesContext()
	{
		Clear();
	}

	void CTilesContext::Clear()
	{
		for (CTile* tile : m_TilesDefinitions)
		{
			delete tile;
			tile = nullptr;
		}

		for (CAtlasTile* tile : m_AtlasTilesDefinitions)
		{
			delete tile;
			tile = nullptr;
		}

		m_TilesDefinitions.clear();
		m_AtlasTilesDefinitions.clear();
	}

	void CTilesContext::LoadTileDefinition(ITile* _tile)
	{
		switch (_tile->GetTileType())
		{
		case ETileType::Regular:
		{
			m_TilesDefinitions.push_back(static_cast<CTile*>(_tile));
			break;
		}
		case ETileType::Atlas:
		{
			m_AtlasTilesDefinitions.push_back(static_cast<CAtlasTile*>(_tile));
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
		return m_TilesDefinitions;
	}

	const std::vector<CAtlasTile*>& CTilesContext::GetAtlasTilesConstRef() const
	{
		return m_AtlasTilesDefinitions;
	}
}