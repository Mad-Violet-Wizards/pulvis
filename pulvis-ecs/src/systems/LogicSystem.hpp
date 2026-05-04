#pragma once

#include "SystemBase.hpp"
#include "components/LogicComponent.hpp"

namespace pulvis::scriptable { class CScriptableService; }

namespace pulvis::ecs
{
  class CSignalScriptBridge;

  class CLogicSystem final : public CSystemBase<SLogicComponent>
  {
  public:

    explicit CLogicSystem(pulvis::scriptable::CScriptableService& _scriptable, CSignalScriptBridge& _signal_bridge);

    void Frame(CWorld& _world, float _delta_time) override;

  private:

    pulvis::scriptable::CScriptableService& m_Scriptable;
    CSignalScriptBridge& m_SignalBridge;
  };
}