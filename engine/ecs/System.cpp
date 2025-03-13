#include "engine/engine_pch.hpp"
#include "System.hpp"
#include "engine/ecs/Component.hpp"
#include "engine/core/UUID.hpp"

namespace engine::ecs
{
	template<class C>
	System<C>::~System()
	{
		for (C* component : m_Components)
		{
			delete component;
			component = nullptr;
		}
	}

	template<class C>
	inline void System<C>::RegisterComponent(C* _component)
	{
		static_assert(std::is_base_of<IComponent, C>::value, "C must derive from IComponent");
		m_Components.push_back(_component);
	}

	template<class C>
	inline void System<C>::RemoveComponent(C* _component)
	{
		static_assert(std::is_base_of<IComponent, C>::value, "C must derive from IComponent");
		auto it = std::find(m_Components.begin(), m_Components.end(), _component);
		if (it != m_Components.end())
		{
			m_Components.erase(it);
		}
	}

	template<class C>
	C* System<C>::GetComponent(const engine::core::UUID& _id)
	{
		for (C* component : m_Components)
		{
			if (component->GetID() == _id)
			{
				return component;
			}
		}

		return nullptr;
	}

	template<class C>
	inline void System<C>::Frame(float _dt)
	{
		for (C* component : m_Components)
		{
			component->Frame(_dt);
		}
	}
}