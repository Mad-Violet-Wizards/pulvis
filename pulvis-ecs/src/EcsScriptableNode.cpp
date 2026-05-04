#include "EcsScriptableNode.hpp"
#include "EcsWorld.hpp"
#include "Entity.hpp"
#include "TemplateCache.hpp"
#include "components/LogicComponent.hpp"
#include "components/HierarchyComponent.hpp"
#include "systems/HierarchySystem.hpp"

namespace pulvis::ecs
{
  CEcsScriptableNode::CEcsScriptableNode(CWorld& _world, CTemplateCache& _registry)
    : m_World(_world), m_Registry(_registry)
  {
  }

  void CEcsScriptableNode::RegisterLuaBindings(sol::state& _luaState)
  {
    _luaState.new_usertype<SEntityHandle>("EntityHandle",
      "index", &SEntityHandle::Index,
      "generation", &SEntityHandle::Generation,
      "is_valid", &SEntityHandle::IsValid);

    _luaState.new_usertype<CEntity>("Entity",
      "handle", &CEntity::GetHandle,
      "is_valid", &CEntity::IsValid,
      "destroy", &CEntity::Destroy);

    _luaState.new_usertype<CWorld>("EcsWorld",
      "create_entity", &CWorld::CreateEntity);

    _luaState.new_usertype<CTemplateCache>("TemplateCache",
      "instantiate", &CTemplateCache::Instantiate,
      "instantiate_template", &CTemplateCache::InstantiateTemplate,
      "find_template_by_name", &CTemplateCache::FindTemplateIdByName,
      "find_preset_by_name", &CTemplateCache::FindPresetIdByName);

    _luaState["pulvis_world"] = &m_World;
    _luaState["pulvis_template_registry"] = &m_Registry;

    _luaState.set_function("pulvis_hierarchy_attach",
      [this](CEntity _parent, CEntity _child, uint8_t _layer)
      {
        const SEntityHandle parent = _parent.GetHandle();
        const SEntityHandle child = _child.GetHandle();
        m_World.Commands().Record(
          [parent, child, _layer](CWorld& _w)
          {
            CHierarchySystem::AttachChild(_w, parent, child, _layer);
          });
      });

    _luaState.set_function("pulvis_hierarchy_detach",
      [this](CEntity _entity)
      {
        const SEntityHandle handle = _entity.GetHandle();
        m_World.Commands().Record(
          [handle](CWorld& _w)
          {
            CHierarchySystem::DetachFromParent(_w, handle);
          });
      });
  }
}