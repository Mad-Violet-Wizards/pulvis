#pragma once

#include "assets/AssetLoader.hpp"

namespace pulvis::systems::animation
{
	class CAnimationEventRegistry;

	class CAnimationEventsLoader final : public pulvis::fs::assets::IAssetLoader
	{
	public:

		explicit CAnimationEventsLoader(CAnimationEventRegistry& _registry);

		[[nodiscard]] bool Process(pulvis::fs::assets::SAssetEntry& _entry) override;
		[[nodiscard]] bool Unload(pulvis::fs::assets::SAssetEntry& _entry) override;

	private:

		CAnimationEventRegistry& m_Registry;
	};
} // namespace pulvis::systems::animation