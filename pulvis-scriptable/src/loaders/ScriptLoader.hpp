#pragma once

#include "assets/AssetLoader.hpp"

namespace pulvis::scriptable
{
	class CScriptLoader final : public pulvis::fs::assets::IAssetLoader
	{
	public:

		bool Process(pulvis::fs::assets::SAssetEntry& _entry) override;
		bool Unload(pulvis::fs::assets::SAssetEntry& _entry) override;
	};
} // namespace pulvis::scriptable