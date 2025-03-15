#include "engine/engine_pch.hpp"
#include "ResourceService.hpp"

#include "TilesContext.hpp"
#include "TilesScriptable.hpp"
#include "engine/scriptable/ScriptableService.hpp"

namespace engine::resources
{
	CResourceService::CResourceService()
	{
	}

	CResourceService::~CResourceService()
	{
	}

	void CResourceService::RegisterRTTI()
	{
		(void)CTile::s_ClassRegistrar_CTile;
		(void)CAtlasTile::s_ClassRegistrar_CAtlasTile;
	}

	void CResourceService::LoadTileDefinitions()
	{
		scriptable::CScriptableService::GetInstance().InvokeScript("tile_definitions.lua");
	}

	void CResourceService::LoadTileDefinition(ITile* _tile)
	{
		m_TilesContext.LoadTileDefinition(_tile);
	}
}