#pragma once

#include "Event.hpp"
#include "EventDispatcher.hpp"

#include <sol3/sol.hpp>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace pulvis::rtti
{
	class CRTTIClass;
} // namespace pulvis::rtti

namespace pulvis::events
{
	//////////////////////////////////////////////////////////////////////////
	class CEventScriptBridge final
	{
	public:

		using event_factory_t = std::function<std::unique_ptr<IEvent>()>;

		CEventScriptBridge(CEventDispatcher& _dispatcher, sol::state& _lua);

		void RegisterEvent(
			event_id_t                       _event_id,
			std::string                      _script_name,
			const pulvis::rtti::CRTTIClass* _rtti_class,
			event_factory_t                  _factory);

		[[nodiscard]] sol::table GetApiTable() const { return m_ApiTable; }

	private:

		struct SRegistration
		{
			event_id_t                      EventId = 0;
			std::string                     Name;
			const pulvis::rtti::CRTTIClass* RttiClass = nullptr;
			event_factory_t                 Factory;
			std::vector<sol::function>      ScriptListeners;
		};

		void OnNativeDispatched(event_id_t _event_id, const IEvent& _event);

		void Lua_Subscribe(const std::string& _name, sol::function _handler);
		void Lua_Submit(const std::string& _name, sol::table _payload);

		static sol::table BuildPayloadTable(sol::state_view _lua, const pulvis::rtti::CRTTIClass& _class, const void* _instance);
		static void       ApplyPayloadTable(const sol::table& _src, const pulvis::rtti::CRTTIClass& _class, void* _instance);

	private:

		CEventDispatcher& m_Dispatcher;
		sol::state& m_Lua;
		sol::table                                     m_ApiTable;
		std::unordered_map<std::string, event_id_t>    m_ByName;
		std::unordered_map<event_id_t, SRegistration>  m_Registrations;
	};
} // namespace pulvis::events