#pragma once

#include "EntityHandle.hpp"
#include "animation/AnimationTypes.hpp"
#include "components/DirectionComponent.hpp"

#include <glm/vec4.hpp>

namespace pulvis::ecs { class CWorld; }
namespace pulvis::rendering { class CRenderLayerCache; }

namespace pulvis::systems::vis
{
	class IVisBinding
	{
	public:

		virtual ~IVisBinding() = default;

		virtual void Detach(pulvis::ecs::CWorld& _world) = 0;

		virtual void SetVisible(pulvis::ecs::CWorld& _world, bool _visible) = 0;
		virtual void SetTint(pulvis::ecs::CWorld& _world, const glm::vec4& _tint) = 0;

		virtual void PlaySequence(pulvis::ecs::CWorld& _world,
			pulvis::systems::animation::anim_id_t     _animation,
			pulvis::systems::animation::sequence_id_t _sequence) = 0;

		[[nodiscard]] virtual pulvis::ecs::SEntityHandle GetEntity() const noexcept = 0;
	};
} // namespace pulvis::systems::vis