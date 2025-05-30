#include "engine/engine_pch.hpp"
#include "ResourceService.hpp"

#include "TilesContext.hpp"
#include "ScriptTile.hpp"
#include "engine/scriptable/ScriptableService.hpp"

namespace engine::resources
{
	CResourceService::CResourceService()
	{
	}

	CResourceService::~CResourceService()
	{
	}

	void CResourceService::LoadTileDefinitions()
	{
		scriptable::CScriptableService::GetInstance().InvokeScript("tile_definitions.lua");
	}

	void CResourceService::LoadTileDefinition(IScriptTile* _tile)
	{
		m_TilesContext.LoadTileDefinition(_tile);
	}
}