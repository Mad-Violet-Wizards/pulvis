#pragma once

#include "CommandBuffer.hpp"
#include "EcsWorld.hpp"

namespace pulvis::ecs
{
	template<typename Fn>
	void CCommandBuffer::RecordCreate(Fn&& _on_created)
	{
		Record([fn = std::forward<Fn>(_on_created)](CWorld& _world) mutable
			{
				const SEntityHandle handle = _world.CreateEntityImmediate();
				fn(_world, handle);
			});
	}
} // namespace pulvis::ecs