#pragma once

#include "assets/AssetLoader.hpp"
#include "assets/AssetRegistry.hpp"

namespace pulvis::fs::assets
{
	class CAnimationLoader : public IAssetLoader
	{
	public:

		explicit CAnimationLoader(CAssetRegistry& _registry);

		bool Process(SAssetEntry& _entry) override;
		bool Unload(SAssetEntry& _entry) override;

	private:

		CAssetRegistry& m_Registry;
	};
}