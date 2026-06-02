#pragma once

#include "AnimationTypes.hpp"
#include "AnimationRegistry.hpp"
#include "AnimationEventRegistry.hpp"
#include "AnimationEventManager.hpp"
#include "EntityHandle.hpp"

#include <memory>

namespace pulvis::ecs { class CWorld; }
namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::systems::animation
{
	class CAnimationService final
	{
	public:

		CAnimationService();
		~CAnimationService();

		CAnimationService(const CAnimationService&) = delete;
		CAnimationService& operator=(const CAnimationService&) = delete;

		void Initialize(pulvis::fs::assets::CAssetRegistry& _asset_registry);
		void Shutdown();

		[[nodiscard]] CAnimationRegistry& GetRegistry()            noexcept { return *m_Registry; }
		[[nodiscard]] const CAnimationRegistry& GetRegistry()      const noexcept { return *m_Registry; }
		[[nodiscard]] CAnimationEventRegistry& GetEventRegistry()       noexcept { return *m_EventRegistry; }
		[[nodiscard]] const CAnimationEventRegistry& GetEventRegistry() const noexcept { return *m_EventRegistry; }
		[[nodiscard]] CAnimationEventManager& GetEventManager()        noexcept { return *m_EventManager; }
		[[nodiscard]] const CAnimationEventManager& GetEventManager()  const noexcept { return *m_EventManager; }

		bool PlaySequence(pulvis::ecs::CWorld& _world,
			pulvis::ecs::SEntityHandle _entity,
			anim_id_t                  _animation,
			sequence_id_t              _sequence) const;

	private:

		std::unique_ptr<CAnimationRegistry>      m_Registry;
		std::unique_ptr<CAnimationEventRegistry> m_EventRegistry;
		std::unique_ptr<CAnimationEventManager>  m_EventManager;

		bool m_Initialized = false;
	};
} // namespace pulvis::systems::animation