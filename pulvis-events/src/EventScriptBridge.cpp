#include "EventScriptBridge.hpp"

#include "RTTIClass.hpp"
#include "RTTIField.hpp"
#include "RTTIAttributes.hpp"
#include "RTTITypeInfo.hpp"

#include <string>

namespace pulvis::events
{
	//////////////////////////////////////////////////////////////////////////
	CEventScriptBridge::CEventScriptBridge(CEventDispatcher& _dispatcher, sol::state& _lua)
		: m_Dispatcher(_dispatcher)
		, m_Lua(_lua)
	{
		m_ApiTable = m_Lua.create_named_table("events");
		m_ApiTable.set_function("subscribe",
			[this](const std::string& _name, sol::function _h) { Lua_Subscribe(_name, std::move(_h)); });
		m_ApiTable.set_function("submit",
			[this](const std::string& _name, sol::table _p) { Lua_Submit(_name, std::move(_p)); });
	}

	void CEventScriptBridge::RegisterEvent(
		event_id_t                       _event_id,
		std::string                      _script_name,
		const pulvis::rtti::CRTTIClass* _rtti_class,
		event_factory_t                  _factory)
	{
		if (_rtti_class == nullptr)
		{
			return;
		}

		SRegistration reg;
		reg.EventId = _event_id;
		reg.Name = _script_name;
		reg.RttiClass = _rtti_class;
		reg.Factory = std::move(_factory);

		m_ByName[_script_name] = _event_id;
		m_Registrations[_event_id] = std::move(reg);

		m_Dispatcher.RegisterScriptHookListener(_event_id,
			[this, _event_id](const IEvent& _event) { OnNativeDispatched(_event_id, _event); });
	}

	void CEventScriptBridge::OnNativeDispatched(event_id_t _event_id, const IEvent& _event)
	{
		auto it = m_Registrations.find(_event_id);
		if (it == m_Registrations.end() || it->second.ScriptListeners.empty())
		{
			return;
		}

		sol::table payload = BuildPayloadTable(sol::state_view(m_Lua), *it->second.RttiClass, &_event);
		for (auto& listener : it->second.ScriptListeners)
		{
			listener(payload);
		}
	}

	void CEventScriptBridge::Lua_Subscribe(const std::string& _name, sol::function _handler)
	{
		auto name_it = m_ByName.find(_name);
		if (name_it == m_ByName.end())
		{
			return;
		}
		m_Registrations[name_it->second].ScriptListeners.push_back(std::move(_handler));
	}

	void CEventScriptBridge::Lua_Submit(const std::string& _name, sol::table _payload)
	{
		auto name_it = m_ByName.find(_name);
		if (name_it == m_ByName.end())
		{
			return;
		}
		auto& reg = m_Registrations[name_it->second];
		if (!reg.Factory)
		{
			return;
		}

		auto event = reg.Factory();
		ApplyPayloadTable(_payload, *reg.RttiClass, event.get());

		// Runtime-typed dispatch: type known only at runtime (factory-produced).
		// Allowed because we are the friend bridge for script -> native pathway.
		m_Dispatcher.Dispatch(*event);
	}

	sol::table CEventScriptBridge::BuildPayloadTable(sol::state_view _lua, const pulvis::rtti::CRTTIClass& _class, const void* _instance)
	{
		sol::table out = _lua.create_table();

		for (const pulvis::rtti::CRTTIField* field : _class.GetFields())
		{
			if (!field->HasAttribute(pulvis::rtti::ERTTIFieldAttribute::Scriptable))
			{
				continue;
			}

			const std::any value = field->GetByAny(_instance);
			if (!value.has_value())
			{
				continue;
			}

			switch (field->GetFieldType())
			{
			case pulvis::rtti::ERTTIFieldType::Int:    out[field->GetName()] = std::any_cast<int>(value);         break;
			case pulvis::rtti::ERTTIFieldType::Uint32: out[field->GetName()] = std::any_cast<uint32_t>(value);    break;
			case pulvis::rtti::ERTTIFieldType::Float:  out[field->GetName()] = std::any_cast<float>(value);       break;
			case pulvis::rtti::ERTTIFieldType::Double: out[field->GetName()] = std::any_cast<double>(value);      break;
			case pulvis::rtti::ERTTIFieldType::Bool:   out[field->GetName()] = std::any_cast<bool>(value);        break;
			case pulvis::rtti::ERTTIFieldType::String: out[field->GetName()] = std::any_cast<std::string>(value); break;
			default: break;
			}
		}
		return out;
	}

	void CEventScriptBridge::ApplyPayloadTable(const sol::table& _src, const pulvis::rtti::CRTTIClass& _class, void* _instance)
	{
		for (const pulvis::rtti::CRTTIField* field : _class.GetFields())
		{
			if (!field->HasAttribute(pulvis::rtti::ERTTIFieldAttribute::Scriptable))
			{
				continue;
			}
			sol::object value = _src[field->GetName()];
			if (!value.valid())
			{
				continue;
			}

			switch (field->GetFieldType())
			{
			case pulvis::rtti::ERTTIFieldType::Int:    field->SetByAny(_instance, std::any(value.as<int>()));         break;
			case pulvis::rtti::ERTTIFieldType::Uint32: field->SetByAny(_instance, std::any(value.as<uint32_t>()));    break;
			case pulvis::rtti::ERTTIFieldType::Float:  field->SetByAny(_instance, std::any(value.as<float>()));       break;
			case pulvis::rtti::ERTTIFieldType::Double: field->SetByAny(_instance, std::any(value.as<double>()));      break;
			case pulvis::rtti::ERTTIFieldType::Bool:   field->SetByAny(_instance, std::any(value.as<bool>()));        break;
			case pulvis::rtti::ERTTIFieldType::String: field->SetByAny(_instance, std::any(value.as<std::string>())); break;
			default: break;
			}
		}
	}
} // namespace pulvis::events