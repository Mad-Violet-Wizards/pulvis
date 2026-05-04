#include "systems/LogicSystem.hpp"
#include "EcsWorld.hpp"
#include "ScriptableService.hpp"
#include "Logger.hpp"
#include "sigslot/SignalScriptBridge.hpp"

namespace pulvis::ecs
{
  CLogicSystem::CLogicSystem(pulvis::scriptable::CScriptableService& _scriptable, CSignalScriptBridge& _signal_bridge)
    : CSystemBase<SLogicComponent>("LogicSystem", ESystemPhase::Script, /*thread_safe=*/false)
    , m_Scriptable(_scriptable)
    , m_SignalBridge(_signal_bridge)
  {
  }

  void CLogicSystem::Frame(CWorld& _world, float _delta_time)
  {
    auto* storage = _world.GetStorage<SLogicComponent>();
    if (!storage)
    {
      return;
    }

    const component_id_t logic_comp_id = GetComponentId<SLogicComponent>();

    storage->ForEach([&](SEntityHandle _entity, SLogicComponent& _logic)
      {
        if (!_logic.Enabled)
        {
          return;
        }

        if (!_logic.SetupCalled && _logic.Setup.valid())
        {
          if (_logic.Self.valid())
          {
            m_SignalBridge.InstallSelfApi(_logic.Self, _entity, logic_comp_id);
          }

          sol::protected_function_result result = _logic.Setup(_logic.Self, _entity.Index);
          if (!result.valid())
          {
            sol::error err = result;
            PULVIS_ERROR_LOG("LogicComponent setup() failed: {}", err.what());
          }
          _logic.SetupCalled = true;
        }

        if (_logic.Frame.valid())
        {
          sol::protected_function_result result = _logic.Frame(_logic.Self, _delta_time);
          if (!result.valid())
          {
            sol::error err = result;
            PULVIS_ERROR_LOG("LogicComponent frame() failed: {}", err.what());
          }
        }
      });
  }
} // namespace pulvis::ecs