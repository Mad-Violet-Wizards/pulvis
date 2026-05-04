#pragma once

#include "EntityHandle.hpp"
#include "EcsTypes.hpp"

#include "FastFunction.hpp"

#include <vector>
#include <mutex>
#include <utility>

namespace pulvis::ecs
{
	class CWorld;

	class CCommandBuffer final
	{
	public:

		template<typename TFn>
		void Record(TFn&& _fn)
		{
			std::lock_guard lock(m_Mutex);
			m_Commands.emplace_back(
				pulvis::tl::FastFunction::Make<void, CWorld&>(std::forward<TFn>(_fn)));
		}

		void Record(pulvis::tl::FastFunction _fn)
		{
			std::lock_guard lock(m_Mutex);
			m_Commands.emplace_back(std::move(_fn));
		}

		template<typename Fn>
		void RecordCreate(Fn&& _on_created);

		void RecordDestroy(SEntityHandle _entity_handle);
		void RecordRemoveComponent(SEntityHandle _entity_handle, component_id_t _component_id);

		void Flush(CWorld& _world);
		void Clear();

		[[nodiscard]] size_t Size() const;

	private:

		mutable std::mutex m_Mutex;
		std::vector<pulvis::tl::FastFunction> m_Commands;
	};
} // namespace pulvis::ecs