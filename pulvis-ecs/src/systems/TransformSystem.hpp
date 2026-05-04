#pragma once

#include "SystemBase.hpp"
#include "EntityHandle.hpp"
#include "components/TransformComponent.hpp"

namespace pulvis::ecs
{
	class CTransformSystem final : public CSystemBase<STransformComponent>
	{
	public:

		CTransformSystem();

		void Frame(CWorld& _world, float _delta_time) override;

	private:

		void PropagateRecursive(CWorld& _world, SEntityHandle _entity,
			const glm::vec2& _parent_position,
			const glm::vec2& _parent_scale,
			float            _parent_rotation,
			bool             _parent_dirty);
	};
} // namespace pulvis::ecs