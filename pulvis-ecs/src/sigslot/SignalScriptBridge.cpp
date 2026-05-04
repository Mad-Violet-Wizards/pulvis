#include "SignalScriptBridge.hpp"

#include <cmath>
#include <limits>

namespace pulvis::ecs
{
	//////////////////////////////////////////////////////////////////////////
	CSignalScriptBridge::CSignalScriptBridge(CSignalCache& _registry)
		: m_Registry(_registry)
	{
	}

	void CSignalScriptBridge::RegisterUserTypes(sol::state& _lua)
	{
		_lua.new_usertype<SSignalConnection>("SignalConnection",
			"is_valid", &SSignalConnection::IsValid,
			"source", sol::readonly(&SSignalConnection::Source),
			"source_comp", sol::readonly(&SSignalConnection::SourceComponent),
			"signal", sol::readonly(&SSignalConnection::Signal),
			"slot", sol::readonly(&SSignalConnection::Slot));

		_lua.new_usertype<SEntityHandle>("EntityHandle",
			"index", sol::readonly(&SEntityHandle::Index),
			"generation", sol::readonly(&SEntityHandle::Generation),
			"is_valid", &SEntityHandle::IsValid);
	}

	void CSignalScriptBridge::InstallSelfApi(sol::table _self, SEntityHandle _entity, component_id_t _logic_component_id)
	{
		_self["entity"] = _entity;

		_self.set_function("emit_signal",
			[this, _entity, _logic_component_id](sol::table /*self*/, const std::string& _component_name, const std::string& _signal_name, sol::variadic_args _args)
			{
				(void)_component_name;
				EmitFromScript(_entity, _logic_component_id, _signal_name, _args);
			});

		_self.set_function("connect_signal",
			[this, _entity, _logic_component_id](sol::table /*self*/, const std::string& _component_name, const std::string& _signal_name, sol::function _handler)
			{
				(void)_component_name;
				return ConnectScript(_entity, _logic_component_id, _signal_name, std::move(_handler), _entity);
			});
	}

	SSignalConnection CSignalScriptBridge::ConnectScript(
		SEntityHandle  _source,
		component_id_t _source_component,
		const std::string& _signal_name,
		sol::function _handler,
		SEntityHandle _slot_owner)
	{
		sol::state_view lua_view(_handler.lua_state());

		auto wrapper = [handler = std::move(_handler), lua_view](const SSignalArgs& _args)
			{
				sol::variadic_results results;
				results.reserve(_args.Count());
				for (size_t i = 0; i < _args.Count(); ++i)
				{
					const std::any& v = _args.Values[i];
					sol::object obj = sol::nil;

					if (v.type() == typeid(int32_t)) { obj = sol::make_object(lua_view, std::any_cast<int32_t>(v)); }
					else if (v.type() == typeid(int64_t)) { obj = sol::make_object(lua_view, std::any_cast<int64_t>(v)); }
					else if (v.type() == typeid(uint32_t)) { obj = sol::make_object(lua_view, std::any_cast<uint32_t>(v)); }
					else if (v.type() == typeid(uint64_t)) { obj = sol::make_object(lua_view, std::any_cast<uint64_t>(v)); }
					else if (v.type() == typeid(float)) { obj = sol::make_object(lua_view, std::any_cast<float>(v)); }
					else if (v.type() == typeid(double)) { obj = sol::make_object(lua_view, std::any_cast<double>(v)); }
					else if (v.type() == typeid(bool)) { obj = sol::make_object(lua_view, std::any_cast<bool>(v)); }
					else if (v.type() == typeid(std::string)) { obj = sol::make_object(lua_view, std::any_cast<const std::string&>(v)); }
					else if (v.type() == typeid(const char*)) { obj = sol::make_object(lua_view, std::any_cast<const char*>(v)); }
					else if (v.type() == typeid(SEntityHandle)) { obj = sol::make_object(lua_view, std::any_cast<SEntityHandle>(v)); }

					results.push_back(obj);
				}
				handler(sol::as_args(results));
			};

		return m_Registry.Connect(_source, _source_component, HashSignalName(_signal_name),
			std::move(wrapper), _slot_owner);
	}

	void CSignalScriptBridge::EmitFromScript(
		SEntityHandle  _source,
		component_id_t _source_component,
		const std::string& _signal_name,
		sol::variadic_args _args)
	{
		SSignalArgs pack;
		pack.Values.reserve(_args.size());
		for (auto it = _args.cbegin(); it != _args.cend(); ++it)
		{
			pack.Values.emplace_back(LuaObjectToAny(*it));
		}
		m_Registry.EmitByName(_source, _source_component, _signal_name, pack);
	}

	std::any CSignalScriptBridge::LuaObjectToAny(const sol::object& _obj)
	{
		switch (_obj.get_type())
		{
		case sol::type::boolean: return std::any(_obj.as<bool>());
		case sol::type::number:
		{
			const double d = _obj.as<double>();
			const double whole = std::trunc(d);
			if (whole == d
				&& d >= static_cast<double>(std::numeric_limits<int64_t>::min())
				&& d <= static_cast<double>(std::numeric_limits<int64_t>::max()))
			{
				return std::any(static_cast<int64_t>(d));
			}
			return std::any(d);
		}
		case sol::type::string: return std::any(_obj.as<std::string>());
		default: return std::any();
		}
	}
} // namespace pulvis::ecs