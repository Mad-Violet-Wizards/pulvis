#pragma once

#include "SystemBase.hpp"
#include "components/SpriteComponent.hpp"

namespace pulvis::rendering
{
	class CRenderQueue;
	class CRenderLayerCache;
}

namespace pulvis::ecs
{
	class CSpriteRenderSystem final : public CSystemBase<SSpriteComponent>
	{
	public:

		CSpriteRenderSystem(pulvis::rendering::CRenderQueue& _queue,
			pulvis::rendering::CRenderLayerCache& _layer_cache);

		void Frame(CWorld& _world, float _delta_time) override;

	private:

		pulvis::rendering::CRenderQueue& m_Queue;
		pulvis::rendering::CRenderLayerCache& m_LayerCache;
	};
} // namespace pulvis::ecs