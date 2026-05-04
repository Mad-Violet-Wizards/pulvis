#include "TemplateScriptableNode.hpp"

#include "EcsWorld.hpp"
#include "TemplateCache.hpp"
#include "ComponentBlueprint.hpp"
#include "Template.hpp"
#include "Preset.hpp"
#include "IComponentStorage.hpp"
#include "ComponentTraits.hpp"
#include "sigslot/SignalCache.hpp"
#include "TemplateEvents.hpp"

#include "EventDispatcher.hpp"
#include "Logger.hpp"

#include <utility>
#include <vector>

namespace pulvis::ecs
{
	namespace
	{
		std::any LuaToAny(const sol::object& _obj)
		{
			switch (_obj.get_type())
			{
			case sol::type::boolean: return std::any(_obj.as<bool>());
			case sol::type::number:
			{
				const double d = _obj.as<double>();
				const double truncated = static_cast<double>(static_cast<int64_t>(d));
				if (d == truncated) { return std::any(static_cast<int64_t>(d)); }
				return std::any(d);
			}
			case sol::type::string: return std::any(_obj.as<std::string>());
			default: return {};
			}
		}

		void FlattenFields(const sol::table& _table,
			const std::string& _prefix,
			std::vector<std::pair<std::string, std::any>>& _out)
		{
			for (const auto& kv : _table)
			{
				if (!kv.first.is<std::string>()) { continue; }
				const std::string key = kv.first.as<std::string>();
				const std::string path = _prefix.empty() ? key : (_prefix + "." + key);
				const sol::object& value = kv.second;

				if (value.is<sol::table>())
				{
					FlattenFields(value.as<sol::table>(), path, _out);
					continue;
				}

				std::any any_value = LuaToAny(value);
				if (any_value.has_value())
				{
					_out.emplace_back(std::move(path), std::move(any_value));
				}
			}
		}
	}

	CTemplateScriptableNode::CTemplateScriptableNode(CWorld& _world,
		CTemplateCache& _template_cache,
		CSignalCache& _signal_cache,
		pulvis::events::CEventDispatcher& _event_dispatcher)
		: m_World(_world)
		, m_Cache(_template_cache)
		, m_SignalCache(_signal_cache)
		, m_EventDispatcher(_event_dispatcher)
	{
	}

	component_id_t CTemplateScriptableNode::ResolveComponentId(std::string_view _type_name) const
	{
		const size_t count = m_World.StorageCount();
		for (size_t i = 0; i < count; ++i)
		{
			IComponentStorage* storage = const_cast<CWorld&>(m_World).GetStorageById(static_cast<component_id_t>(i));
			if (storage == nullptr) { continue; }
			if (storage->Traits().TypeName == _type_name)
			{
				return static_cast<component_id_t>(i);
			}
		}
		return INVALID_COMPONENT_ID;
	}

	void CTemplateScriptableNode::RegisterLuaBindings(sol::state& _lua_state)
	{
		sol::table pulvis_tbl = _lua_state["pulvis"].get_or_create<sol::table>();
		sol::table ecs_tbl = pulvis_tbl["ecs"].get_or_create<sol::table>();
		sol::table tmpl_tbl = ecs_tbl["template"].get_or_create<sol::table>();
		sol::table preset_tbl = ecs_tbl["preset"].get_or_create<sol::table>();

		tmpl_tbl.set_function("register",
			[this](const std::string& _name, sol::table _desc)
			{
				RegisterTemplateFromLua(_name, _desc);
			});

		preset_tbl.set_function("register",
			[this](const std::string& _name, sol::table _desc)
			{
				RegisterPresetFromLua(_name, _desc);
			});
	}

	void CTemplateScriptableNode::RegisterTemplateFromLua(const std::string& _name, const sol::table& _desc)
	{
		STemplate tmpl;
		tmpl.Name = _name;
		tmpl.ScriptPath = _desc.get_or("script_path", std::string{});

		sol::optional<sol::table> components_opt = _desc["components"];
		if (components_opt)
		{
			for (const auto& kv : components_opt.value())
			{
				if (!kv.second.is<sol::table>()) { continue; }
				sol::table entry = kv.second.as<sol::table>();
				const std::string type_name = entry.get_or("type", std::string{});
				if (type_name.empty()) { continue; }

				const component_id_t comp_id = ResolveComponentId(type_name);
				if (comp_id == INVALID_COMPONENT_ID)
				{
					PULVIS_WARNING_LOG("Template '{}': unknown component type '{}'.", _name, type_name);
					continue;
				}

				SComponentBlueprint bp;
				bp.ComponentId = comp_id;
				bp.TypeName = type_name;

				std::vector<std::pair<std::string, std::any>> flat_fields;
				sol::optional<sol::table> fields_opt = entry["fields"];
				if (fields_opt)
				{
					FlattenFields(fields_opt.value(), std::string{}, flat_fields);
				}

				bp.Factory = [comp_id, flat_fields](CWorld& _world, SEntityHandle _entity)
					{
						IComponentStorage* storage = _world.GetStorageById(comp_id);
						if (storage == nullptr) { return; }
						storage->EmplaceDefault(_entity);
						for (const auto& [field_path, value] : flat_fields)
						{
							_world.SetComponentField(_entity, comp_id, field_path, value);
						}
					};

				tmpl.Components.push_back(std::move(bp));
			}
		}

		sol::optional<sol::table> bindings_opt = _desc["signal_bindings"];
		if (bindings_opt)
		{
			for (const auto& kv : bindings_opt.value())
			{
				if (!kv.second.is<sol::table>()) { continue; }
				sol::table b = kv.second.as<sol::table>();

				STemplateSignalBinding binding;
				const std::string source_type = b.get_or("source_component", std::string{});
				binding.SignalName = b.get_or("signal_name", std::string{});
				binding.TargetScriptHandler = b.get_or("target_handler", std::string{});

				binding.SourceComponent = ResolveComponentId(source_type);
				if (binding.SourceComponent != INVALID_COMPONENT_ID && !binding.SignalName.empty())
				{
					tmpl.SignalBindings.push_back(std::move(binding));
				}
			}
		}

		const template_id_t id = m_Cache.RegisterTemplate(std::move(tmpl));

		STemplateRegisteredEvent evt;
		evt.Name = _name;
		evt.TemplateId = id;
		m_EventDispatcher.Submit<STemplateRegisteredEvent>(std::move(evt));
	}

	void CTemplateScriptableNode::RegisterPresetFromLua(const std::string& _name, const sol::table& _desc)
	{
		SPreset preset;
		preset.Name = _name;

		const std::string base_name = _desc.get_or("base_template", std::string{});
		preset.BaseTemplate = m_Cache.FindTemplateIdByName(base_name);
		if (preset.BaseTemplate == INVALID_TEMPLATE_ID)
		{
			PULVIS_ERROR_LOG("Preset '{}': base template '{}' not found.", _name, base_name);
			return;
		}

		sol::optional<sol::table> overrides_opt = _desc["overrides"];
		if (overrides_opt)
		{
			for (const auto& kv : overrides_opt.value())
			{
				if (!kv.second.is<sol::table>()) { continue; }
				sol::table o = kv.second.as<sol::table>();

				const std::string comp_type = o.get_or("component", std::string{});
				const std::string field = o.get_or("field", std::string{});
				sol::object value = o["value"];
				if (comp_type.empty() || field.empty()) { continue; }

				const component_id_t comp_id = ResolveComponentId(comp_type);
				if (comp_id == INVALID_COMPONENT_ID) { continue; }

				SPresetFieldOverride ov;
				ov.ComponentId = comp_id;
				ov.FieldName = field;
				ov.Value = LuaToAny(value);
				if (ov.Value.has_value())
				{
					preset.Overrides.push_back(std::move(ov));
				}
			}
		}

		const preset_id_t id = m_Cache.RegisterPreset(std::move(preset));

		SPresetRegisteredEvent evt;
		evt.Name = _name;
		evt.PresetId = id;
		m_EventDispatcher.Submit<SPresetRegisteredEvent>(std::move(evt));
	}
} // namespace pulvis::ecs