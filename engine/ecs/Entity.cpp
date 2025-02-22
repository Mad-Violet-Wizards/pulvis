#include "engine/engine_pch.hpp"
#include "engine/ecs/Entity.hpp"
#include "engine/ecs/System.hpp"
#include "engine/ecs/Component.hpp"

namespace engine::ecs
{
	CEntity::CEntity(CEntity* _parent)
		: m_Parent(_parent)
	{
	}

	template<class C>
	inline void CEntity::CreateComponent(C* _component)
	{
		static_assert(std::is_base_of<IComponent, C>::value, "C must derive from IComponent");
		m_ComponentIDs.push_back(_component->GetID());
		System<C>::RegisterComponent(_component);
	}

	void CEntity::AttachChild(CEntity* _child)
	{
		m_Children.push_back(_child);
	}
}