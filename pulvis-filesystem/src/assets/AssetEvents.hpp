#pragma once

#include "Event.hpp"
#include "FileTypes.hpp"
#include "assets/AssetEntry.hpp"

#include <string>

namespace pulvis::fs::assets
{
	struct SAssetLoadedEvent : public pulvis::events::TEvent<SAssetLoadedEvent>
	{
		SAssetHandle Handle;
		std::string  VirtualPath;
		EAssetType   Type = EAssetType::Unknown;
	};

	struct SAssetUnloadedEvent : public pulvis::events::TEvent<SAssetUnloadedEvent>
	{
		SAssetHandle Handle;
		std::string  VirtualPath;
		EAssetType   Type = EAssetType::Unknown;
	};

	struct SAssetReloadedEvent : public pulvis::events::TEvent<SAssetReloadedEvent>
	{
		SAssetHandle Handle;
		std::string  VirtualPath;
		EAssetType   Type = EAssetType::Unknown;
	};
} // namespace pulvis::fs::assets::events