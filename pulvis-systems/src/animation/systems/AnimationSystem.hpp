#pragma once

#include "SystemBase.hpp"
#include "EntityHandle.hpp"
#include "animation/components/AnimationStateComponent.hpp"
#include "animation/AnimationFrameContext.hpp"

#include "components/SpriteComponent.hpp"
#include "animation/AnimationSequence.hpp"
#include "animation/Animation.hpp"

#include <vector>

namespace pulvis::systems::animation
{
	class CAnimationEventManager;

	class CAnimationSystem final : public pulvis::ecs::CSystemBase<SAnimationStateComponent>
	{
	public:

		explicit CAnimationSystem(CAnimationEventManager& _event_manager);

		void Frame(pulvis::ecs::CWorld& _world, float _delta_time) override;

		[[nodiscard]] const CAnimationEventManager& GetEventManager() const noexcept { return m_EventManager; }
		[[nodiscard]] CAnimationEventManager& GetEventManager() noexcept { return m_EventManager; }

	private:

		frame_id_t FrameAtTimeMs(const SAnimationSequence& _seq, float _t_ms) const;
		void UpdateSprite(pulvis::ecs::SSpriteComponent& _sprite, const SAnimation& _anim, const CAnimationFrame& _frame, EAnimationFlags _flags);

	private:

		CAnimationEventManager& m_EventManager;
		std::vector<SAnimationFrameContext> m_PendingEvents;
	};
} // namespace pulvis::systems::animation