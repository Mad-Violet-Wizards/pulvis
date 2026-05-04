#pragma once

#include "ScriptableNode.hpp"
#include "EcsTypes.hpp"

#include <sol3/sol.hpp>
#include <string_view>
#include <string>

namespace pulvis::events { class CEventDispatcher; }

namespace pulvis::ecs
{
	class CTemplateCache;
	class CComponentTypeRegistry;
	class CSignalCache;
	class CWorld;
	/*
		Lua API: pulvis.ecs.template.register(name, desc)
						 pulvis.ecs.preset.register(name, desc)

		File-level loading is handled by CTemplateLoader / CPresetLoader through CAssetRegistry. 
		This node only owns the script-side surface and emits
		STemplateRegisteredEvent / SPresetRegisteredEvent so loaders can correlate scripts with cache ids.
	*/
	class CTemplateScriptableNode : public pulvis::scriptable::IScriptableNode
	{
	public:

		CTemplateScriptableNode(CWorld& _world,
			CTemplateCache& _template_cache,
			CSignalCache& _signal_cache,
			pulvis::events::CEventDispatcher& _event_dispatcher);

		void RegisterLuaBindings(sol::state& _lua_state) override;

	private:

		void RegisterTemplateFromLua(const std::string& _name, const sol::table& _desc);
		void RegisterPresetFromLua(const std::string& _name, const sol::table& _desc);

		[[nodiscard]] component_id_t ResolveComponentId(std::string_view _type_name) const;

	private:

		CWorld& m_World;
		CTemplateCache& m_Cache;
		CSignalCache& m_SignalCache;
		pulvis::events::CEventDispatcher& m_EventDispatcher;
	};
} // namespace pulvis::ecs