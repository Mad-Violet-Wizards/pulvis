#pragma once

#include "SignalCache.hpp"

#include <sol3/sol.hpp>

namespace pulvis::ecs
{
	//////////////////////////////////////////////////////////////////////////
	class CSignalScriptBridge final
	{
	public:

		explicit CSignalScriptBridge(CSignalCache& _registry);

		void RegisterUserTypes(sol::state& _lua);

		void InstallSelfApi(sol::table _self, SEntityHandle _entity, component_id_t _logic_component_id);

		[[nodiscard]] SSignalConnection ConnectScript(
			SEntityHandle  _source,
			component_id_t _source_component,
			const std::string& _signal_name,
			sol::function _handler,
			SEntityHandle _slot_owner = {});

		void EmitFromScript(
			SEntityHandle  _source,
			component_id_t _source_component,
			const std::string& _signal_name,
			sol::variadic_args _args);

	private:

		static std::any LuaObjectToAny(const sol::object& _obj);

	private:

		CSignalCache& m_Registry;
	};
} // namespace pulvis::ecs