#pragma once

#include <sol3/sol.hpp>
#include <string>

namespace pulvis::ecs
{
  struct SLogicComponent final
  {
    std::string    ScriptPath;
    sol::function  Setup;
    sol::function  Frame;
    sol::table     Self;
    bool           SetupCalled = false;
    bool           Enabled = true;
  };
}