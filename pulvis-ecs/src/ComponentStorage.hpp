#pragma once

#include "IComponentStorage.hpp"
#include "PoolAllocator.hpp"
#include "EntityHandle.hpp"
#include "Assert.hpp"

#include <limits>
#include <type_traits>
#include <vector>

namespace pulvis::ecs
{
	//////////////////////////////////////////////////////////////////////////
	template<typename TComponent>
	class CComponentStorage final : public IComponentStorage
	{
	public:

		explicit CComponentStorage(SComponentTraits _traits)
			: m_Traits(std::move(_traits))
		{
			m_Traits.Size = sizeof(TComponent);
			m_Traits.Alignment = alignof(TComponent);
		}

		template<typename... Args>
		TComponent& Emplace(SEntityHandle _entity, Args&&... _args)
		{
			ASSERT(!Has(_entity), "Entity already has this component.");
			const size_t dense_idx = m_Allocator.Size();
			m_Allocator.EmplaceBack(std::forward<Args>(_args)...);
			m_Entities.push_back(_entity);
			EnsureSparse(_entity.Index);
			m_EntityToIndex[_entity.Index] = static_cast<uint32_t>(dense_idx);
			return m_Allocator.At(dense_idx);
		}

		void EmplaceDefault(SEntityHandle _entity) override
		{
			if constexpr (std::is_default_constructible_v<TComponent>)
			{
				Emplace(_entity);
			}
			else
			{
				ASSERT(false, "Component is not default-constructible; cannot apply blueprint.");
			}
		}

		void RemoveIfPresent(SEntityHandle _entity_handle) override
		{
			Remove(_entity_handle);
		}

		void Remove(SEntityHandle _entity_handle)
		{
			if (!Has(_entity_handle))
			{
				return;
			}

			uint32_t dense_index = m_EntityToIndex[_entity_handle.Index];
			const size_t last_index = static_cast<uint32_t>(m_Allocator.Size() - 1);

			if (!m_Traits.IsStable && dense_index != last_index)
			{
				m_Allocator.At(dense_index) = std::move(m_Allocator.At(last_index));
				const SEntityHandle moved = m_Entities[last_index];
				m_Entities[dense_index] = moved;
				m_EntityToIndex[moved.Index] = dense_index;
			}

			m_Allocator.PopBack();
			m_Entities.pop_back();
			m_EntityToIndex[_entity_handle.Index] = INVALID_DENSE_INDEX;
		}

		[[nodiscard]] bool Has(SEntityHandle _entity_handle) const override
		{
			return _entity_handle.Index < m_EntityToIndex.size() && m_EntityToIndex[_entity_handle.Index] != INVALID_DENSE_INDEX;
		}

		[[nodiscard]] void* TryGetRaw(SEntityHandle _entity) override
		{
			return Has(_entity) ? static_cast<void*>(&m_Allocator.At(m_EntityToIndex[_entity.Index])) : nullptr;
		}

		[[nodiscard]] TComponent* TryGet(SEntityHandle _entity)
		{
			return Has(_entity) ? &m_Allocator.At(m_EntityToIndex[_entity.Index]) : nullptr;
		}

		[[nodiscard]] const TComponent* TryGet(SEntityHandle _entity) const
		{
			return Has(_entity) ? &m_Allocator.At(m_EntityToIndex[_entity.Index]) : nullptr;
		}

		[[nodiscard]] TComponent& Get(SEntityHandle _entity_handle)
		{
			ASSERT(Has(_entity_handle), "Entity does not have this component.");
			return m_Allocator.At(m_EntityToIndex[_entity_handle.Index]);
		}

		template<typename Fn>
		void ForEach(Fn&& _fn)
		{
			for (size_t i = 0; i < m_Allocator.Size(); ++i)
			{
				_fn(m_Entities[i], m_Allocator.At(i));
			}
		}

		[[nodiscard]] size_t Count() const override { return m_Allocator.Size(); }
		[[nodiscard]] component_id_t GetTypeId() const override { return GetComponentId<TComponent>(); }
		[[nodiscard]] SComponentTraits& Traits() override { return m_Traits; }
		[[nodiscard]] const SComponentTraits& Traits() const override { return m_Traits; }

		void Clear() override
		{
			m_Allocator.Clear();
			m_Entities.clear();
			m_EntityToIndex.clear();
		}

		[[nodiscard]] std::vector<SEntityHandle>& GetEntities() { return m_Entities; }
		[[nodiscard]] const std::vector<SEntityHandle>& GetEntities() const { return m_Entities; }

	private:

		static constexpr uint32_t INVALID_DENSE_INDEX = std::numeric_limits<uint32_t>::max();

		void EnsureSparse(entity_index_t _index)
		{
			if (_index >= m_EntityToIndex.size())
			{
				m_EntityToIndex.resize(_index + 1, INVALID_DENSE_INDEX);
			}
		}

	private:

		CPoolAllocator<TComponent> m_Allocator;
		std::vector<SEntityHandle> m_Entities;
		std::vector<uint32_t> m_EntityToIndex;
		SComponentTraits m_Traits;
	};
} // namespace pulvis::ecs