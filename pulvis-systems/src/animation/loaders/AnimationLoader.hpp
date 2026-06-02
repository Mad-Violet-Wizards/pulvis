#pragma once

#include "assets/AssetLoader.hpp"

namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::systems::animation
{
	class CAnimationRegistry;
	class CAnimationEventRegistry;

	class CAnimationLoader final : public pulvis::fs::assets::IAssetLoader
	{
	public:

		CAnimationLoader(CAnimationRegistry& _registry,
			CAnimationEventRegistry& _event_registry,
			pulvis::fs::assets::CAssetRegistry& _asset_registry);

		[[nodiscard]] bool Process(pulvis::fs::assets::SAssetEntry& _entry) override;
		[[nodiscard]] bool Unload(pulvis::fs::assets::SAssetEntry& _entry) override;

	private:

		CAnimationRegistry& m_Registry;
		CAnimationEventRegistry& m_EventRegistry;
		pulvis::fs::assets::CAssetRegistry& m_AssetRegistry;
	};
} // namespace pulvis::systems::animation