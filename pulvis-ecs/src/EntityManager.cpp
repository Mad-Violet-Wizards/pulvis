#include "EntityManager.hpp"

namespace pulvis::ecs
{
  CEntityManager::CEntityManager() = default;

  SEntityHandle CEntityManager::Create()
  {
    std::lock_guard lock(m_Mutex);

    if (!m_FreeList.empty())
    {
      const entity_index_t idx = m_FreeList.back();
      m_FreeList.pop_back();
      m_Free[idx] = false;
      ++m_AliveCount;
      return SEntityHandle{ idx, m_Generations[idx] };
    }

    const entity_index_t idx = static_cast<entity_index_t>(m_Generations.size());
    m_Generations.push_back(1);
    m_Free.push_back(false);
    ++m_AliveCount;
    return SEntityHandle{ idx, 1 };
  }

  void CEntityManager::Destroy(SEntityHandle _entity)
  {
    std::lock_guard lock(m_Mutex);

    if (_entity.Index >= m_Generations.size()) return;
    if (m_Free[_entity.Index]) return;
    if (m_Generations[_entity.Index] != _entity.Generation) return;

    ++m_Generations[_entity.Index];
    if (m_Generations[_entity.Index] == INVALID_ENTITY_GENERATION)
    {
      m_Generations[_entity.Index] = 1;
    }
    m_Free[_entity.Index] = true;
    m_FreeList.push_back(_entity.Index);
    --m_AliveCount;
  }

  bool CEntityManager::IsAlive(SEntityHandle _entity) const
  {
    std::lock_guard lock(m_Mutex);
    if (_entity.Index >= m_Generations.size()) return false;
    return !m_Free[_entity.Index] && m_Generations[_entity.Index] == _entity.Generation;
  }

  size_t CEntityManager::AliveCount() const
  {
    std::lock_guard lock(m_Mutex);
    return m_AliveCount;
  }

  size_t CEntityManager::Capacity() const
  {
    std::lock_guard lock(m_Mutex);
    return m_Generations.size();
  }
}