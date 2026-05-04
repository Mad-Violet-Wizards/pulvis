#pragma once

#include "EntityManager.hpp"
#include "ComponentStorage.hpp"
#include "ComponentTraits.hpp"
#include "CommandBuffer.hpp"
#include "ComponentBlueprint.hpp"
#include "ComponentTypeId.hpp"
#include "Template.hpp"
#include "Assert.hpp"

#include "RTTIClass.hpp"
#include "RTTIField.hpp"
#include "RTTITypeInfo.hpp"

#include <any>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace pulvis::ecs
{
	class CSignalCache;
	class CEntity;

	//////////////////////////////////////////////////////////////////////////
	class CWorld final
	{
	public:

		CWorld();
		~CWorld();

		CWorld(const CWorld&) = delete;
		CWorld& operator=(const CWorld&) = delete;

		template<typename TComponent>
		void RegisterComponent(SComponentTraits _traits = {})
		{
			const component_id_t id = GetComponentId<TComponent>();
			if (id >= m_Storages.size())
			{
				m_Storages.resize(id + 1);
			}
			if (m_Storages[id])
			{
				return;
			}
			if (_traits.TypeName.empty())
			{
				_traits.TypeName = typeid(TComponent).name();
			}
			if (!_traits.SetFieldByName)
			{
				_traits.SetFieldByName = &CWorld::SetTypedFieldByName<TComponent>;
			}
			m_Storages[id] = std::make_unique<CComponentStorage<TComponent>>(std::move(_traits));
		}

		template<typename TComponent>
		[[nodiscard]] CComponentStorage<TComponent>* GetStorage()
		{
			const component_id_t id = GetComponentId<TComponent>();
			if (id >= m_Storages.size() || !m_Storages[id])
			{
				return nullptr;
			}

			return static_cast<CComponentStorage<TComponent>*>(m_Storages[id].get());
		}

		[[nodiscard]] IComponentStorage* GetStorageById(component_id_t _id)
		{
			return _id < m_Storages.size() ? m_Storages[_id].get() : nullptr;
		}

		[[nodiscard]] SEntityHandle CreateEntity();
		void DestroyEntity(SEntityHandle _entity);

		template<typename TComponent, typename... TArgs>
		TComponent& AddComponent(SEntityHandle _entity, TArgs&&... _args)
		{
			CComponentStorage<TComponent>* storage = GetStorage<TComponent>();
			ASSERT(storage, "Component type not registered");
			TComponent& ref = storage->Emplace(_entity, std::forward<TArgs>(_args)...);
			const auto& hooks = storage->Traits().LifecycleHooks;
			if (hooks.OnAttach)
			{
				hooks.OnAttach(*this, _entity, &ref);
			}
			return ref;
		}

		template<typename TComponent>
		void RemoveComponent(SEntityHandle _entity)
		{
			RemoveComponent(_entity, GetComponentId<TComponent>());
		}

		void RemoveComponent(SEntityHandle _entity, component_id_t _component_id);

		template<typename TComponent>
		[[nodiscard]] TComponent* TryGetComponent(SEntityHandle _entity)
		{
			auto* storage = GetStorage<TComponent>();
			return storage ? storage->TryGet(_entity) : nullptr;
		}

		template<typename TComponent>
		[[nodiscard]] const TComponent* TryGetComponent(SEntityHandle _entity) const
		{
			auto* storage = const_cast<CWorld*>(this)->GetStorage<TComponent>();
			return storage ? storage->TryGet(_entity) : nullptr;
		}

		void AddComponentFromBlueprint(SEntityHandle _entity, const SComponentBlueprint& _bp);
		void OverrideComponentHooks(SEntityHandle _entity, component_id_t _component_id, const SComponentHookOverride& _override);
		void SetComponentField(SEntityHandle _entity, component_id_t _component_id, std::string_view _field_name, const std::any& _value);

		[[nodiscard]] CCommandBuffer& Commands() { return m_Commands; }
		void FlushCommands();

		[[nodiscard]] CEntity Wrap(SEntityHandle _handle);

		[[nodiscard]] CEntityManager& Entities() { return m_EntityManager; }
		[[nodiscard]] const CEntityManager& Entities() const { return m_EntityManager; }
		[[nodiscard]] bool IsAlive(SEntityHandle _entity) const { return m_EntityManager.IsAlive(_entity); }

		[[nodiscard]] size_t StorageCount() const { return m_Storages.size(); }

		void SetSignalRegistry(CSignalCache* _registry) { m_SignalRegistry = _registry; }
		[[nodiscard]] CSignalCache* GetSignalRegistry() const { return m_SignalRegistry; }

	private:

		template<typename TComponent>
		static void SetTypedFieldByName(void* _raw, std::string_view _field_name, const std::any& _value)
		{
			auto* typed = static_cast<TComponent*>(_raw);
			const auto* cls = pulvis::rtti::CRTTIClass::FindConstInStorage(
				pulvis::rtti::CRTTITypeInfo<TComponent>::GetTypeId());
			if (cls == nullptr)
			{
				return;
			}
			const auto* field = cls->FindConstFieldByName(std::string(_field_name).c_str());
			if (field == nullptr)
			{
				return;
			}
			DispatchSet<TComponent>(typed, field, _value);
		}

		template<typename TComponent>
		static void DispatchSet(TComponent* _typed, const pulvis::rtti::CRTTIField* _field, const std::any& _value)
		{
			using pulvis::rtti::ERTTIFieldType;
			switch (_field->GetFieldType())
			{
			case ERTTIFieldType::Int:    SetCoerced<TComponent, int>(_typed, _field, _value);         break;
			case ERTTIFieldType::Uint32: SetCoerced<TComponent, uint32_t>(_typed, _field, _value);    break;
			case ERTTIFieldType::Float:  SetCoerced<TComponent, float>(_typed, _field, _value);       break;
			case ERTTIFieldType::Double: SetCoerced<TComponent, double>(_typed, _field, _value);      break;
			case ERTTIFieldType::Bool:   SetCoerced<TComponent, bool>(_typed, _field, _value);        break;
			case ERTTIFieldType::String: SetCoerced<TComponent, std::string>(_typed, _field, _value); break;
			default: break;
			}
		}

		template<typename TComponent, typename TField>
		static void SetCoerced(TComponent* _typed, const pulvis::rtti::CRTTIField* _field, const std::any& _value)
		{
			if (_value.type() == typeid(TField))
			{
				_field->Set(_typed, std::any_cast<TField>(_value));
				return;
			}
			if constexpr (std::is_arithmetic_v<TField>)
			{
				if (_value.type() == typeid(int64_t)) { _field->Set(_typed, static_cast<TField>(std::any_cast<int64_t>(_value)));  return; }
				if (_value.type() == typeid(int)) { _field->Set(_typed, static_cast<TField>(std::any_cast<int>(_value)));      return; }
				if (_value.type() == typeid(uint32_t)) { _field->Set(_typed, static_cast<TField>(std::any_cast<uint32_t>(_value))); return; }
				if (_value.type() == typeid(uint64_t)) { _field->Set(_typed, static_cast<TField>(std::any_cast<uint64_t>(_value))); return; }
				if (_value.type() == typeid(double)) { _field->Set(_typed, static_cast<TField>(std::any_cast<double>(_value)));   return; }
				if (_value.type() == typeid(float)) { _field->Set(_typed, static_cast<TField>(std::any_cast<float>(_value)));    return; }
				if (_value.type() == typeid(bool)) { _field->Set(_typed, static_cast<TField>(std::any_cast<bool>(_value)));     return; }
			}
		}

	private:

		CEntityManager                                                    m_EntityManager;
		std::vector<std::unique_ptr<IComponentStorage>>                   m_Storages;
		CCommandBuffer                                                    m_Commands;
		CSignalCache* m_SignalRegistry = nullptr;
		std::unordered_map<uint64_t, std::vector<SComponentHookOverride>> m_HookOverrides;
	};
} // namespace pulvis::ecs