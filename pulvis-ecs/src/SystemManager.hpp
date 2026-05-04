#pragma once

#include "ISystem.hpp"

#include <memory>
#include <vector>
#include <array>

namespace pulvis::threads { class CJobSystem; }

namespace pulvis::ecs
{
  class CWorld;

  class CSystemManager final
  {
  public:

    CSystemManager() = default;

    void Register(std::unique_ptr<ISystem> _system);
    void SetJobSystem(pulvis::threads::CJobSystem* _job_system) { m_JobSystem = _job_system; }

    void Frame(CWorld& _world, float _delta_time);

  private:

    void RunPhaseSequential(CWorld& _world, float _delta_time, const std::vector<ISystem*>& _systems);
    void RunPhaseParallel(CWorld& _world, float _delta_time, const std::vector<ISystem*>& _systems);

    [[nodiscard]] static bool Conflicts(const ISystem& _lhs, const ISystem& _rhs);

  private:

    std::array<std::vector<std::unique_ptr<ISystem>>, static_cast<size_t>(ESystemPhase::Count)> m_Systems;
    pulvis::threads::CJobSystem* m_JobSystem = nullptr;
  };
}