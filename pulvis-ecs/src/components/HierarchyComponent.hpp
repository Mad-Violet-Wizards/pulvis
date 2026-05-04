#pragma once

#include "EntityHandle.hpp"

namespace pulvis::ecs
{
  struct SHierarchyComponent final
  {
    SEntityHandle     Parent;
    SEntityHandle     FirstChild;
    SEntityHandle     NextSibling;
    SEntityHandle     PrevSibling;
    hierarchy_layer_t Layer = 0;
    uint16_t          ChildCount = 0;
  };
}