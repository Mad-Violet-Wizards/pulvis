#include "systems/HierarchySystem.hpp"
#include "EcsWorld.hpp"

#include <algorithm>

namespace pulvis::ecs
{
  CHierarchySystem::CHierarchySystem()
    : CSystemBase<SHierarchyComponent>("HierarchySystem", ESystemPhase::Hierarchy, /*thread_safe=*/false)
  {
  }

  void CHierarchySystem::Frame(CWorld& _world, float _delta_time)
  {
    auto* storage = _world.GetStorage<SHierarchyComponent>();
    if (!storage)
    {
      return;
    }

    std::vector<SEntityHandle> roots;
    storage->ForEach([&](SEntityHandle _entity, SHierarchyComponent& _node)
      {
        if (!_node.Parent.IsValid())
        {
          roots.push_back(_entity);
        }
      });

    for (SEntityHandle root : roots)
    {
      VisitSubtree(_world, root, _delta_time);
    }
  }

  void CHierarchySystem::VisitSubtree(CWorld& _world, SEntityHandle _root, float _delta_time)
  {
    std::vector<SEntityHandle> children;
    CollectSortedChildren(_world, _root, children);
    for (SEntityHandle child : children)
    {
      VisitSubtree(_world, child, _delta_time);
    }
  }

  void CHierarchySystem::CollectSortedChildren(CWorld& _world, SEntityHandle _parent, std::vector<SEntityHandle>& _out)
  {
    auto* storage = _world.GetStorage<SHierarchyComponent>();
    if (!storage)
    {
      return;
    }

    const SHierarchyComponent* parent_node = storage->TryGet(_parent);
    if (!parent_node)
    {
      return;
    }

    SEntityHandle cur = parent_node->FirstChild;
    while (cur.IsValid())
    {
      _out.push_back(cur);
      const SHierarchyComponent* node = storage->TryGet(cur);
      cur = node ? node->NextSibling : SEntityHandle{};
    }

    std::stable_sort(_out.begin(), _out.end(),
      [storage](SEntityHandle _a, SEntityHandle _b)
      {
        const auto* an = storage->TryGet(_a);
        const auto* bn = storage->TryGet(_b);
        return (an ? an->Layer : 0) < (bn ? bn->Layer : 0);
      });
  }

  void CHierarchySystem::AttachChild(CWorld& _world, SEntityHandle _parent, SEntityHandle _child, hierarchy_layer_t _layer)
  {
    auto* storage = _world.GetStorage<SHierarchyComponent>();
    if (!storage)
    {
      return;
    }

    if (!storage->Has(_child))
    {
      _world.AddComponent<SHierarchyComponent>(_child);
    }
    if (!storage->Has(_parent))
    {
      _world.AddComponent<SHierarchyComponent>(_parent);
    }

    DetachFromParent(_world, _child);

    SHierarchyComponent& child_node = storage->Get(_child);
    SHierarchyComponent& parent_node = storage->Get(_parent);

    child_node.Parent = _parent;
    child_node.Layer = _layer;
    child_node.NextSibling = parent_node.FirstChild;
    child_node.PrevSibling = SEntityHandle{};

    if (parent_node.FirstChild.IsValid())
    {
      storage->Get(parent_node.FirstChild).PrevSibling = _child;
    }
    parent_node.FirstChild = _child;
    ++parent_node.ChildCount;
  }

  void CHierarchySystem::DetachFromParent(CWorld& _world, SEntityHandle _entity)
  {
    auto* storage = _world.GetStorage<SHierarchyComponent>();
    if (!storage || !storage->Has(_entity))
    {
      return;
    }
    SHierarchyComponent& node = storage->Get(_entity);
    if (!node.Parent.IsValid())
    {
      return;
    }
    SHierarchyComponent& parent_node = storage->Get(node.Parent);

    if (node.PrevSibling.IsValid())
    {
      storage->Get(node.PrevSibling).NextSibling = node.NextSibling;
    }
    else
    {
      parent_node.FirstChild = node.NextSibling;
    }
    if (node.NextSibling.IsValid())
    {
      storage->Get(node.NextSibling).PrevSibling = node.PrevSibling;
    }

    --parent_node.ChildCount;
    node.Parent = SEntityHandle{};
    node.NextSibling = SEntityHandle{};
    node.PrevSibling = SEntityHandle{};
  }
}