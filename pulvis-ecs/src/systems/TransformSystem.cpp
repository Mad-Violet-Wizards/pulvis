#include "systems/TransformSystem.hpp"

#include "EcsWorld.hpp"
#include "components/HierarchyComponent.hpp"

#include <cmath>

namespace pulvis::ecs
{
	CTransformSystem::CTransformSystem()
		: CSystemBase<STransformComponent>("TransformSystem", ESystemPhase::Trasnform, /*thread_safe=*/false)
	{
	}

	void CTransformSystem::Frame(CWorld& _world, float /*_delta_time*/)
	{
		auto* transform_storage = _world.GetStorage<STransformComponent>();
		if (transform_storage == nullptr)
		{
			return;
		}

		auto* hierarchy_storage = _world.GetStorage<SHierarchyComponent>();

		transform_storage->ForEach([&](SEntityHandle _entity, STransformComponent& _t)
			{
				const SHierarchyComponent* node = hierarchy_storage ? hierarchy_storage->TryGet(_entity) : nullptr;
				if (node != nullptr && node->Parent.IsValid())
				{
					return;
				}
				PropagateRecursive(_world, _entity,
					{ 0.f, 0.f }, { 1.f, 1.f }, 0.f, false);
			});
	}

	void CTransformSystem::PropagateRecursive(CWorld& _world, SEntityHandle _entity,
		const glm::vec2& _parent_position,
		const glm::vec2& _parent_scale,
		float            _parent_rotation,
		bool             _parent_dirty)
	{
		auto* transform_storage = _world.GetStorage<STransformComponent>();
		auto* hierarchy_storage = _world.GetStorage<SHierarchyComponent>();

		STransformComponent* transform = transform_storage->TryGet(_entity);
		if (transform == nullptr)
		{
			return;
		}

		const bool needs_update = _parent_dirty || transform->Dirty;
		if (needs_update)
		{
			const float cos_r = std::cos(_parent_rotation);
			const float sin_r = std::sin(_parent_rotation);

			const glm::vec2 scaled_local =
			{
				transform->LocalPosition.x * _parent_scale.x,
				transform->LocalPosition.y * _parent_scale.y
			};

			const glm::vec2 rotated_local =
			{
				scaled_local.x * cos_r - scaled_local.y * sin_r,
				scaled_local.x * sin_r + scaled_local.y * cos_r
			};

			transform->WorldPosition = _parent_position + rotated_local;
			transform->WorldScale = { _parent_scale.x * transform->LocalScale.x,
																	 _parent_scale.y * transform->LocalScale.y };
			transform->WorldRotation = _parent_rotation + transform->LocalRotation;
			transform->Dirty = false;
		}

		if (hierarchy_storage == nullptr)
		{
			return;
		}

		const SHierarchyComponent* node = hierarchy_storage->TryGet(_entity);
		if (node == nullptr)
		{
			return;
		}

		SEntityHandle child = node->FirstChild;
		while (child.IsValid())
		{
			PropagateRecursive(_world, child,
				transform->WorldPosition,
				transform->WorldScale,
				transform->WorldRotation,
				needs_update);

			const SHierarchyComponent* child_node = hierarchy_storage->TryGet(child);
			child = child_node ? child_node->NextSibling : SEntityHandle{};
		}
	}
} // namespace pulvis::ecs