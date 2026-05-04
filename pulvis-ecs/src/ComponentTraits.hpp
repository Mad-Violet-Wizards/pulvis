#pragma once

#include "EntityHandle.hpp"

#include <string>
#include <string_view>
#include <functional>
#include <unordered_map>
#include <vector>
#include <any>

namespace pulvis::ecs
{
	class CWorld;

	//////////////////////////////////////////////////////////////////////////
	// Lifecycle hook called when component is attached/detached.
	using component_lifecycle_fn_t = std::function<void(CWorld&, SEntityHandle, void*)>;

	// Custom (user-named) hook with arbitrary argument bag.
	using component_custom_hook_fn_t = std::function<void(CWorld&, SEntityHandle, void*, const std::vector<std::any>&)>;

	//////////////////////////////////////////////////////////////////////////
	struct SLifecycleHooks final
	{
		component_lifecycle_fn_t OnAttach;
		component_lifecycle_fn_t OnDetach;
	};

	//////////////////////////////////////////////////////////////////////////
	// Copyable on purpose - hooks are shared, immutable callables that can be
	// freely cloned from templates into per-entity overrides.
	class CHookRegistry final
	{
	public:

		CHookRegistry() = default;
		~CHookRegistry() = default;

		CHookRegistry(const CHookRegistry&) = default;
		CHookRegistry& operator=(const CHookRegistry&) = default;

		CHookRegistry(CHookRegistry&&) noexcept = default;
		CHookRegistry& operator=(CHookRegistry&&) noexcept = default;

		template<typename TFn>
		void Set(std::string _name, TFn&& _fn)
		{
			m_Hooks[std::move(_name)] = component_custom_hook_fn_t(std::forward<TFn>(_fn));
		}

		void Invoke(std::string_view _name, CWorld& _world, SEntityHandle _entity, void* _component, const std::vector<std::any>& _args = {}) const
		{
			auto it = m_Hooks.find(std::string(_name));

			if (it == m_Hooks.end() || !it->second)
			{
				return;
			}

			it->second(_world, _entity, _component, _args);
		}

		void MergeFrom(const CHookRegistry& _other)
		{
			for (const auto& [name, fn] : _other.m_Hooks)
			{
				m_Hooks[name] = fn;
			}
		}

		void MergeFrom(CHookRegistry&& _other)
		{
			for (auto& [name, fn] : _other.m_Hooks)
			{
				m_Hooks[name] = std::move(fn);
			}
			_other.m_Hooks.clear();
		}

		void Clear() { m_Hooks.clear(); }
		[[nodiscard]] size_t Count() const { return m_Hooks.size(); }

	private:

		std::unordered_map<std::string, component_custom_hook_fn_t> m_Hooks;
	};

	//////////////////////////////////////////////////////////////////////////
	using component_field_setter_t = std::function<void(void* /*raw*/, std::string_view /*field*/, const std::any& /*value*/)>;

	struct SComponentTraits final
	{
		std::string              TypeName;
		size_t                   Size = 0;
		size_t                   Alignment = 0;
		bool                     IsTag = false;
		bool                     IsStable = false;
		SLifecycleHooks          LifecycleHooks;
		CHookRegistry            CustomHooks;
		component_field_setter_t SetFieldByName;
	};
} // namespace pulvis::ecs