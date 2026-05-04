#pragma once

#include "ScriptableNode.hpp"

namespace pulvis::systems::input
{
  class CInputService;

  class CInputScriptableNode final : public pulvis::scriptable::IScriptableNode
  {
  public:

    explicit CInputScriptableNode(CInputService& _service);

    void RegisterLuaBindings(sol::state& _luaState) override;

  private:

    CInputService& m_Service;
  };
} // namespace pulvis::systems::input