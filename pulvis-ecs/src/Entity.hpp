#pragma once

#include "EcsWorld.hpp"

namespace pulvis::ecs
{
	class CEntity final
	{
		public:

			CEntity() = default;
			CEntity(SEntityHandle _handle, CWorld& _world)
				: m_Handle(_handle)
				, m_World(&_world) { }

			[[nodiscard]] SEntityHandle GetHandle() const { return m_Handle; }
			[[nodiscard]] CWorld* GetWorld() const { return m_World; }
			[[nodiscard]] bool IsValid() const { return m_World && m_World->IsAlive(m_Handle); }

			template<typename TComponent, typename... Args>
			TComponent& AddComponent(Args&&... _args)
			{
				return m_World->AddComponent<TComponent>(m_Handle, std::forward<Args>(_args)...);
			}

			template<typename TComponent>
			void RemoveComponent()
			{
				m_World->RemoveComponent<TComponent>(m_Handle);
			}

			template<typename TComponent>
			[[nodiscard]] TComponent* TryGet() const
			{
				auto* storage = m_World->GetStorage<TComponent>();
				return storage ? storage->TryGet(m_Handle) : nullptr;
			}

			template<typename TComponent>
			[[nodiscard]] bool HasComponent() const
			{
				auto* storage = m_World->GetStorage<TComponent>();
				return storage ? storage->Has(m_Handle) : false;
			}

			template<typename TComponent>
			void InvokeHook(std::string_view _hook_name, const std::vector<std::any>& _args = {})
			{
				auto* storage = m_World->GetStorage<TComponent>();
				if (!storage)
				{
					return;
				}
				void* component = storage->TryGetRaw(m_Handle);
				if (!component)
				{
					return;
				}
				storage->Traits().CustomHooks.Invoke(_hook_name, *m_World, m_Handle, component, _args);
			}

			void Destroy()
			{
				m_World->Commands().RecordDestroy(m_Handle);
			}

		private:

			SEntityHandle m_Handle;
			CWorld* m_World = nullptr;
	};
}