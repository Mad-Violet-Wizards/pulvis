#pragma once

#include "SystemBase.hpp"
#include "components/HierarchyComponent.hpp"

#include <vector>

namespace pulvis::ecs
{
  class CHierarchySystem final : public CSystemBase<SHierarchyComponent>
  {
  public:

    CHierarchySystem();

    void Frame(CWorld& _world, float _delta_time) override;

    static void AttachChild(CWorld& _world, SEntityHandle _parent, SEntityHandle _child, hierarchy_layer_t _layer = 0);
    static void DetachFromParent(CWorld& _world, SEntityHandle _entity);

  private:

    void VisitSubtree(CWorld& _world, SEntityHandle _root, float _delta_time);
    void CollectSortedChildren(CWorld& _world, SEntityHandle _parent, std::vector<SEntityHandle>& _out);
  };
}