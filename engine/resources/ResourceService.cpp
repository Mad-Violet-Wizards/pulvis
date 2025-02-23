#include "engine/engine_pch.hpp"
#include "ResourceService.hpp"

#include "TilesScriptable.hpp"

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

}