#pragma once

#include "EntityHandle.hpp"
#include <vector>
#include <mutex>

namespace pulvis::ecs
{
	class CEntityManager final
	{
		public:

			CEntityManager();

			[[nodiscard]] SEntityHandle Create();
			void Destroy(SEntityHandle _entity_handle);

			[[nodiscard]] bool IsAlive(SEntityHandle _entity_handle) const;
			[[nodiscard]] size_t AliveCount() const;
			[[nodiscard]] size_t Capacity() const;

			template<typename Fn>
			void ForEachAlive(Fn&& _fn) const
			{
				std::lock_guard lock(m_Mutex);
				for (entity_index_t i = 0; i < m_Generations.size(); ++i)
				{
					if (!m_Free[i])
					{
						_fn(SEntityHandle{ i, m_Generations[i] });
					}
				}
			}

		private:

			mutable std::mutex                m_Mutex;
			std::vector<entity_generation_t>  m_Generations;
			std::vector<bool>                 m_Free;
			std::vector<entity_index_t>       m_FreeList;
			size_t                            m_AliveCount = 0;
	};
}