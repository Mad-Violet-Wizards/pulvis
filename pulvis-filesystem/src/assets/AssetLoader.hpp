#pragma once

#include "AssetEntry.hpp"

namespace pulvis::fs::assets
{
	class IAssetLoader
	{
		public:

			virtual ~IAssetLoader() = default;

			virtual bool Process(SAssetEntry& _entry) = 0;
			virtual bool Unload(SAssetEntry& _entry) = 0;
	};
}