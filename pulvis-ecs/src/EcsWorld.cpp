#include "EcsWorld.hpp"
#include "Entity.hpp"

#include "sigslot/SignalCache.hpp"

namespace pulvis::ecs
{
	//////////////////////////////////////////////////////////////////////////
	static uint64_t PackHookKey(SEntityHandle _e, component_id_t _c)
	{
		return (static_cast<uint64_t>(_e.Index) << 32) ^ static_cast<uint64_t>(_c);
	}

	//////////////////////////////////////////////////////////////////////////
	CWorld::CWorld() = default;
	CWorld::~CWorld() = default;

	SEntityHandle CWorld::CreateEntity()
	{
		return m_EntityManager.Create();
	}

	void CWorld::DestroyEntity(SEntityHandle _entity)
	{
		if (!m_EntityManager.IsAlive(_entity))
		{
			return;
		}

		if (m_SignalRegistry != nullptr)
		{
			m_SignalRegistry->OnEntityDestroyed(_entity);
		}

		for (auto& storage : m_Storages)
		{
			if (!storage || !storage->Has(_entity))
			{
				continue;
			}
			const auto& hooks = storage->Traits().LifecycleHooks;
			if (hooks.OnDetach)
			{
				hooks.OnDetach(*this, _entity, storage->TryGetRaw(_entity));
			}
			storage->RemoveIfPresent(_entity);
		}

		m_HookOverrides.erase(PackHookKey(_entity, INVALID_COMPONENT_ID));
		for (auto it = m_HookOverrides.begin(); it != m_HookOverrides.end(); )
		{
			if ((it->first >> 32) == static_cast<uint64_t>(_entity.Index))
			{
				it = m_HookOverrides.erase(it);
			}
			else
			{
				++it;
			}
		}

		m_EntityManager.Destroy(_entity);
	}

	void CWorld::RemoveComponent(SEntityHandle _entity, component_id_t _component_id)
	{
		auto* storage = GetStorageById(_component_id);
		if (!storage || !storage->Has(_entity))
		{
			return;
		}
		const auto& hooks = storage->Traits().LifecycleHooks;
		if (hooks.OnDetach)
		{
			hooks.OnDetach(*this, _entity, storage->TryGetRaw(_entity));
		}
		storage->RemoveIfPresent(_entity);
	}

	void CWorld::AddComponentFromBlueprint(SEntityHandle _entity, const SComponentBlueprint& _bp)
	{
		IComponentStorage* storage = GetStorageById(_bp.ComponentId);
		ASSERT(storage, "Component storage not registered for blueprint.");

		storage->EmplaceDefault(_entity);

		void* raw = storage->TryGetRaw(_entity);
		ASSERT(raw, "Default emplace did not produce raw pointer.");

		const auto& set_fn = storage->Traits().SetFieldByName;
		if (set_fn)
		{
			for (const auto& [name, value] : _bp.Defaults)
			{
				set_fn(raw, name, value);
			}
		}

		const auto& hooks = storage->Traits().LifecycleHooks;
		if (hooks.OnAttach)
		{
			hooks.OnAttach(*this, _entity, raw);
		}
	}

	void CWorld::OverrideComponentHooks(SEntityHandle _entity, component_id_t _component_id, const SComponentHookOverride& _override)
	{
		m_HookOverrides[PackHookKey(_entity, _component_id)].push_back(_override);
	}

	void CWorld::SetComponentField(SEntityHandle _entity, component_id_t _component_id, std::string_view _field_name, const std::any& _value)
	{
		IComponentStorage* storage = GetStorageById(_component_id);
		ASSERT(storage, "Component storage not registered for field override.");

		void* raw = storage->TryGetRaw(_entity);
		if (raw == nullptr)
		{
			return;
		}

		const auto& set_fn = storage->Traits().SetFieldByName;
		if (set_fn)
		{
			set_fn(raw, _field_name, _value);
		}
	}

	void CWorld::FlushCommands()
	{
		m_Commands.Flush(*this);
	}

	CEntity CWorld::Wrap(SEntityHandle _handle)
	{
		return CEntity(_handle, *this);
	}
} // namespace pulvis::ecs