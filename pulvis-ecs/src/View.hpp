#pragma once

#include "EcsWorld.hpp"

#include <tuple>

namespace pulvis::ecs
{
	template<typename... TComponents>
	class CView final
	{
		explicit CView(CWorld& _world)
			: m_World(_world)
		{
		}
		[[nodiscard]] CWorld& GetWorld() const { return m_World; }
	
	private:
		CWorld& m_World;
	};
}