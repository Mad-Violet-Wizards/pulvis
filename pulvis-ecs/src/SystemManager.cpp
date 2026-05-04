#include "SystemManager.hpp"
#include "EcsWorld.hpp"

#include "JobSystem.hpp"
#include "JobCounter.hpp"

namespace pulvis::ecs
{
	void CSystemManager::Register(std::unique_ptr<ISystem> _system)
	{
		const size_t phase = static_cast<size_t>(_system->Phase());
		m_Systems[phase].emplace_back(std::move(_system));
	}

	void CSystemManager::Frame(CWorld& _world, float _delta_time)
	{
		for (size_t phase = 0; phase < static_cast<size_t>(ESystemPhase::Count); ++phase)
		{
			std::vector<ISystem*> raw;
			raw.reserve(m_Systems[phase].size());

			for (auto& system : m_Systems[phase])
			{
				raw.push_back(system.get());
			}

			const bool force_sequential = (phase == static_cast<size_t>(ESystemPhase::Script))
				|| (phase == static_cast<size_t>(ESystemPhase::Hierarchy))
				|| m_JobSystem == nullptr;

			if (force_sequential)
			{
				RunPhaseSequential(_world, _delta_time, raw);
			}
			else
			{
				RunPhaseParallel(_world, _delta_time, raw);
				m_JobSystem->Wait(nullptr);
			}

			_world.FlushCommands();
		}
	}

	void CSystemManager::RunPhaseSequential(CWorld& _world, float _delta_time, const std::vector<ISystem*>& _systems)
	{
		for (auto* sys : _systems)
		{
			sys->Frame(_world, _delta_time);
		}
	}

  void CSystemManager::RunPhaseParallel(CWorld& _world, float _delta_time, const std::vector<ISystem*>& _systems)
  {
    std::vector<bool> done(_systems.size(), false);
    size_t remaining = _systems.size();

    while (remaining > 0)
    {
      std::vector<ISystem*> wave;
      for (size_t i = 0; i < _systems.size(); ++i)
      {
        if (done[i])
        {
          continue;
        }

        ISystem* candidate = _systems[i];
        if (!candidate->IsThreadSafe())
        {
          if (wave.empty())
          {
            wave.push_back(candidate);
            done[i] = true;
            --remaining;
          }

          break;
        }

        bool ok = true;
        for (auto* in_wave : wave)
        {
          if (Conflicts(*candidate, *in_wave))
          {
            ok = false;
            break;
          }
        }

        if (ok)
        {
          wave.push_back(candidate);
          done[i] = true;
          --remaining;
        }
      }

      if (wave.size() == 1 || m_JobSystem == nullptr)
      {
        for (auto* sys : wave)
        {
          sys->Frame(_world, _delta_time);
        }
        continue;
      }

      pulvis::threads::CJobCounter counter;
      for (auto* sys : wave)
      {
        pulvis::threads::SJob* job = m_JobSystem->CreateJob(
          [sys, &_world, _delta_time](pulvis::threads::SJob*)
          {
            sys->Frame(_world, _delta_time);
          },
          &counter);
        m_JobSystem->Submit(job);
      }

      m_JobSystem->Wait(&counter);
    }
  }

  bool CSystemManager::Conflicts(const ISystem& _a, const ISystem& _b)
  {
    const auto& aw = _a.Writes();
    const auto& bw = _b.Writes();
    const auto& ar = _a.Reads();
    const auto& br = _b.Reads();
    if ((aw & bw).any()) return true;
    if ((aw & br).any()) return true;
    if ((ar & bw).any()) return true;
    return false;
  }
}