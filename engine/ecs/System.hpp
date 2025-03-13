#pragma once

namespace engine::ecs
{
	template<class C>
	class System
	{
	public:

		System() = default;
		~System();

		static void RegisterComponent(C* _component);
		static void RemoveComponent(C* _component);
		static C* GetComponent(const core::UUID& _id);

		static void Frame(float _dt);

	private:

		static std::vector<C*> m_Components;

	};
}
