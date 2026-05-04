#include "CommandBuffer.hpp"
#include "EcsWorld.hpp"

namespace pulvis::ecs
{
  void CCommandBuffer::RecordDestroy(SEntityHandle _entity)
  {
    Record([_entity](CWorld& _world)
      {
        _world.DestroyEntity(_entity);
      });
  }

  void CCommandBuffer::RecordRemoveComponent(SEntityHandle _entity, component_id_t _component_id)
  {
    Record([_entity, _component_id](CWorld& _world)
      {
        _world.RemoveComponent(_entity, _component_id);
      });
  }

  void CCommandBuffer::Flush(CWorld& _world)
  {
    std::vector<pulvis::tl::FastFunction> drained;
    {
      std::lock_guard lock(m_Mutex);
      drained.swap(m_Commands);
    }
    for (auto& cmd : drained)
    {
      cmd.Invoke<void, CWorld&>(_world);
    }
  }

  void CCommandBuffer::Clear()
  {
    std::lock_guard lock(m_Mutex);
    m_Commands.clear();
  }

  size_t CCommandBuffer::Size() const
  {
    std::lock_guard lock(m_Mutex);
    return m_Commands.size();
  }
}