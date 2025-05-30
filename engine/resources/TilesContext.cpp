#include "engine/engine_pch.hpp"
#include "TilesContext.hpp"

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
		for (CScriptTile* tile : m_TilesDefinitions)
		{
			delete tile;
			tile = nullptr;
		}

		for (CScriptAtlasTile* tile : m_AtlasTilesDefinitions)
		{
			delete tile;
			tile = nullptr;
		}

		m_TilesDefinitions.clear();
		m_AtlasTilesDefinitions.clear();
	}

	void CTilesContext::LoadTileDefinition(IScriptTile* _tile)
	{
		switch (_tile->GetTileType())
		{
		case ETileType::Regular:
		{
			m_TilesDefinitions.push_back(static_cast<CScriptTile*>(_tile));
			break;
		}
		case ETileType::Atlas:
		{
			m_AtlasTilesDefinitions.push_back(static_cast<CScriptAtlasTile*>(_tile));
			break;
		}
		default:
		{
			PULVIS_WARNING_LOG("{}", "Unkown tile type!");
			break;
		}
		}
	}

	const std::vector<CScriptTile*>& CTilesContext::GetTilesConstRef() const
	{
		return m_TilesDefinitions;
	}

	const std::vector<CScriptAtlasTile*>& CTilesContext::GetAtlasTilesConstRef() const
	{
		return m_AtlasTilesDefinitions;
	}

	void CTilesContext::FillTilesToLoad(std::vector<IScriptTile*>& _tiles) const
	{
		_tiles.reserve(m_TilesDefinitions.size() + m_AtlasTilesDefinitions.size());

		for (CScriptTile* tile : m_TilesDefinitions)
		{
			_tiles.push_back(tile);
		}

		for (CScriptAtlasTile* tile : m_AtlasTilesDefinitions)
		{
			_tiles.push_back(tile);
		}
	}
}