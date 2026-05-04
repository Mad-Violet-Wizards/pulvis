#pragma once

#include "ScriptableNode.hpp"

namespace pulvis::ecs
{
  class CWorld;
  class CTemplateCache;

  class CEcsScriptableNode final : public pulvis::scriptable::IScriptableNode
  {
  public:

    CEcsScriptableNode(CWorld& _world, CTemplateCache& _registry);

    void RegisterLuaBindings(sol::state& _luaState) override;

  private:

    CWorld& m_World;
    CTemplateCache& m_Registry;
  };
} // namespace pulvis::ecs