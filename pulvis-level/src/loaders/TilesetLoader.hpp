#pragma once

#include "assets/AssetLoader.hpp"
#include "assets/AssetRegistry.hpp"

namespace pulvis::level
{
	class CTilesetLoader : public pulvis::fs::assets::IAssetLoader
	{
	public:

		explicit CTilesetLoader(pulvis::fs::assets::CAssetRegistry& _registry);
		bool Process(pulvis::fs::assets::SAssetEntry& _entry) override;
		bool Unload(pulvis::fs::assets::SAssetEntry& _entry) override;

	private:

		pulvis::fs::assets::CAssetRegistry& m_Registry;
	};
}