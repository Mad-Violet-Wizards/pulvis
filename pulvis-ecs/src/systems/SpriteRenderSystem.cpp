#include "systems/SpriteRenderSystem.hpp"

#include "EcsWorld.hpp"
#include "components/TransformComponent.hpp"

#include "RenderQueue.hpp"
#include "RenderLayerCache.hpp"

namespace pulvis::ecs
{
	CSpriteRenderSystem::CSpriteRenderSystem(pulvis::rendering::CRenderQueue& _queue,
		pulvis::rendering::CRenderLayerCache& _layer_cache)
		: CSystemBase<SSpriteComponent>("SpriteRenderSystem", ESystemPhase::Render, /*thread_safe=*/false)
		, m_Queue(_queue)
		, m_LayerCache(_layer_cache)
	{
	}

	void CSpriteRenderSystem::Frame(CWorld& _world, float /*_delta_time*/)
	{
		auto* sprite_storage = _world.GetStorage<SSpriteComponent>();
		auto* transform_storage = _world.GetStorage<STransformComponent>();
		if (sprite_storage == nullptr || transform_storage == nullptr) { return; }

		sprite_storage->ForEach([&](SEntityHandle _entity, SSpriteComponent& _sprite)
			{
				if (!_sprite.Visible) { return; }
				if (!_sprite.Layer.IsValid()) { return; }

				const STransformComponent* xform = transform_storage->TryGet(_entity);
				if (xform == nullptr) { return; }

				pulvis::rendering::SSpriteDrawCmd cmd{};
				cmd.Position.x = xform->WorldPosition.x - _sprite.Size.x * _sprite.Pivot.x * xform->WorldScale.x;
				cmd.Position.y = xform->WorldPosition.y - _sprite.Size.y * _sprite.Pivot.y * xform->WorldScale.y;
				cmd.Size = { _sprite.Size.x * xform->WorldScale.x, _sprite.Size.y * xform->WorldScale.y };
				cmd.UVRect = _sprite.UVRect;
				cmd.Color = _sprite.Color;
				cmd.TextureID = _sprite.TextureID;
				cmd.Layer = _sprite.Layer;
				cmd.SortOrder = _sprite.SortOrder;

				m_Queue.PushSprite(cmd);
			});
	}
} // namespace pulvis::ecs