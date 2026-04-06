#pragma once

#include "assets/AssetLoader.hpp"
#include "assets/AssetRegistry.hpp"

namespace pulvis::fs::assets
{
	class CTilesetLoader : public IAssetLoader
	{
	public:

		explicit CTilesetLoader(CAssetRegistry& _registry);

		bool Process(SAssetEntry& _entry) override;
		bool Unload(SAssetEntry& _entry) override;

	private:

		CAssetRegistry& m_Registry;
	};
}