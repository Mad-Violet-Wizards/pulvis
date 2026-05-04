#include "TemplateCache.hpp"

#include "EcsWorld.hpp"
#include "sigslot/SignalCache.hpp"
#include "sigslot/SignalScriptBridge.hpp"
#include "components/LogicComponent.hpp"

#include "Assert.hpp"
#include "Logger.hpp"

namespace pulvis::ecs
{
	//////////////////////////////////////////////////////////////////////////
	CTemplateCache::CTemplateCache() = default;

	void CTemplateCache::SetSignalSubsystem(CSignalCache* _registry, CSignalScriptBridge* _script_bridge)
	{
		std::unique_lock lock(m_Mutex);
		m_SignalRegistry = _registry;
		m_SignalBridge = _script_bridge;
	}

	template_id_t CTemplateCache::RegisterTemplate(STemplate _tmpl)
	{
		std::unique_lock lock(m_Mutex);

		if (auto it = m_TemplateByName.find(_tmpl.Name); it != m_TemplateByName.end())
		{
			return it->second;
		}

		const template_id_t id = m_NextTemplateId++;
		_tmpl.Id = id;

		m_TemplateByName[_tmpl.Name] = id;
		m_Templates.emplace(id, std::move(_tmpl));
		return id;
	}

	preset_id_t CTemplateCache::RegisterPreset(SPreset _preset)
	{
		std::unique_lock lock(m_Mutex);

		if (auto it = m_PresetByName.find(_preset.Name); it != m_PresetByName.end())
		{
			return it->second;
		}

		const preset_id_t id = m_NextPresetId++;
		_preset.Id = id;

		m_PresetByName[_preset.Name] = id;
		m_Presets.emplace(id, std::move(_preset));
		return id;
	}

	void CTemplateCache::UnregisterTemplate(template_id_t _id)
	{
		std::unique_lock lock(m_Mutex);
		auto it = m_Templates.find(_id);
		if (it == m_Templates.end()) { return; }
		m_TemplateByName.erase(it->second.Name);
		m_Templates.erase(it);
	}

	void CTemplateCache::UnregisterPreset(preset_id_t _id)
	{
		std::unique_lock lock(m_Mutex);
		auto it = m_Presets.find(_id);
		if (it == m_Presets.end()) { return; }
		m_PresetByName.erase(it->second.Name);
		m_Presets.erase(it);
	}

	const STemplate* CTemplateCache::FindTemplate(template_id_t _id) const
	{
		std::shared_lock lock(m_Mutex);
		auto it = m_Templates.find(_id);
		return it == m_Templates.end() ? nullptr : &it->second;
	}

	const SPreset* CTemplateCache::FindPreset(preset_id_t _id) const
	{
		std::shared_lock lock(m_Mutex);
		auto it = m_Presets.find(_id);
		return it == m_Presets.end() ? nullptr : &it->second;
	}

	template_id_t CTemplateCache::FindTemplateIdByName(const std::string& _name) const
	{
		std::shared_lock lock(m_Mutex);
		auto it = m_TemplateByName.find(_name);
		return it == m_TemplateByName.end() ? INVALID_TEMPLATE_ID : it->second;
	}

	preset_id_t CTemplateCache::FindPresetIdByName(const std::string& _name) const
	{
		std::shared_lock lock(m_Mutex);
		auto it = m_PresetByName.find(_name);
		return it == m_PresetByName.end() ? INVALID_PRESET_ID : it->second;
	}

	CEntity CTemplateCache::InstantiateTemplate(CWorld& _world, template_id_t _tmpl)
	{
		std::shared_lock lock(m_Mutex);
		const STemplate* tmpl_ptr = FindTemplate(_tmpl);
		ASSERT(tmpl_ptr, "Template not found.");
		if (!tmpl_ptr)
		{
			return {};
		}

		CEntity entity = _world.Wrap(_world.CreateEntity());
		for (const auto& bp : tmpl_ptr->Components)
		{
			ApplyBlueprint(_world, entity, bp);
		}

		ApplyHookOverrides(_world, entity, *tmpl_ptr);
		ApplySignalBindings(_world, entity, *tmpl_ptr);

		return entity;
	}

	CEntity CTemplateCache::Instantiate(CWorld& _world, preset_id_t _preset)
	{
		SPreset preset_copy;
		{
			std::shared_lock lock(m_Mutex);
			auto it = m_Presets.find(_preset);
			ASSERT(it != m_Presets.end(), "Preset not found.");
			preset_copy = it->second;
		}

		CEntity entity = InstantiateTemplate(_world, preset_copy.BaseTemplate);

		for (const auto& override_entry : preset_copy.Overrides)
		{
			ApplyOverride(_world, entity, override_entry);
		}

		return entity;
	}

	CEntity CTemplateCache::InstantiateByName(CWorld& _world, const std::string& _name)
	{
		const preset_id_t preset_id = FindPresetIdByName(_name);
		if (preset_id != INVALID_PRESET_ID) { return Instantiate(_world, preset_id); }

		const template_id_t tmpl_id = FindTemplateIdByName(_name);
		if (tmpl_id != INVALID_TEMPLATE_ID) { return InstantiateTemplate(_world, tmpl_id); }

		PULVIS_WARNING_LOG("CTemplateCache::InstantiateByName: '{}' not found.", _name);
		return {};
	}

	void CTemplateCache::ApplyBlueprint(CWorld& _world, CEntity& _entity, const SComponentBlueprint& _bp) const
	{
		_world.AddComponentFromBlueprint(_entity.GetHandle(), _bp);
	}

	void CTemplateCache::ApplyHookOverrides(CWorld& _world, CEntity& _entity, const STemplate& _tmpl) const
	{
		for (auto& [comp_id, override_entry] : _tmpl.HookOverrides)
		{
			_world.OverrideComponentHooks(_entity.GetHandle(), comp_id, override_entry);
		}
	}

	void CTemplateCache::ApplyOverride(CWorld& _world, CEntity& _entity, const SPresetFieldOverride& _ov) const
	{
		_world.SetComponentField(_entity.GetHandle(), _ov.ComponentId, _ov.FieldName, _ov.Value);
	}

	void CTemplateCache::ApplySignalBindings(CWorld& _world, CEntity& _entity, const STemplate& _tmpl) const
	{
		if (_tmpl.SignalBindings.empty())
		{
			return;
		}
		if (m_SignalRegistry == nullptr || m_SignalBridge == nullptr)
		{
			return;
		}

		SLogicComponent* logic = _world.TryGetComponent<SLogicComponent>(_entity.GetHandle());
		if (logic == nullptr || !logic->Self.valid())
		{
			return;
		}

		for (const auto& binding : _tmpl.SignalBindings)
		{
			sol::function handler = logic->Self[binding.TargetScriptHandler];
			if (!handler.valid())
			{
				continue;
			}

			(void)m_SignalBridge->ConnectScript(
				_entity.GetHandle(),
				binding.SourceComponent,
				binding.SignalName,
				handler,
				_entity.GetHandle());
		}
	}
} // namespace pulvis::ecs