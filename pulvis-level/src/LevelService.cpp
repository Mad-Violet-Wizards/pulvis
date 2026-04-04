#include "LevelService.hpp"

namespace pulvis::level
{
	CLevelService::CLevelService(pulvis::fs::assets::CAssetRegistry& _asset_registry, pulvis::fs::CMountSystem& _mount_system)
		: m_AssetRegistry(_asset_registry)
		, m_MountSystem(_mount_system)
	{
	}

	void CLevelService::Initialize()
	{
	}

	void CLevelService::Shutdown()
	{
	}
}