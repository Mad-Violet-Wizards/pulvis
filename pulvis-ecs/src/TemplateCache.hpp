#pragma once

#include "Template.hpp"
#include "Preset.hpp"
#include "Entity.hpp"

#include <shared_mutex>
#include <string>
#include <unordered_map>

namespace pulvis::ecs
{
	class CWorld;
	class CSignalCache;
	class CSignalScriptBridge;

	//////////////////////////////////////////////////////////////////////////
	class CTemplateCache final
	{
	public:

		CTemplateCache();

		void SetSignalSubsystem(CSignalCache* _registry, CSignalScriptBridge* _script_bridge);

		[[nodiscard]] template_id_t RegisterTemplate(STemplate _tmpl);
		[[nodiscard]] preset_id_t   RegisterPreset(SPreset _preset);

		void UnregisterTemplate(template_id_t _id);
		void UnregisterPreset(preset_id_t _id);

		[[nodiscard]] const STemplate* FindTemplate(template_id_t _id) const;
		[[nodiscard]] const SPreset* FindPreset(preset_id_t _id) const;

		[[nodiscard]] template_id_t FindTemplateIdByName(const std::string& _name) const;
		[[nodiscard]] preset_id_t   FindPresetIdByName(const std::string& _name) const;

		[[nodiscard]] CEntity Instantiate(CWorld& _world, preset_id_t _preset);
		[[nodiscard]] CEntity InstantiateByName(CWorld& _world, const std::string& _name);
		[[nodiscard]] CEntity InstantiateTemplate(CWorld& _world, template_id_t _tmpl);

	private:

		void ApplyBlueprint(CWorld& _world, CEntity& _entity, const SComponentBlueprint& _bp) const;
		void ApplyHookOverrides(CWorld& _world, CEntity& _entity, const STemplate& _tmpl) const;
		void ApplyOverride(CWorld& _world, CEntity& _entity, const SPresetFieldOverride& _ov) const;
		void ApplySignalBindings(CWorld& _world, CEntity& _entity, const STemplate& _tmpl) const;

	private:

		mutable std::shared_mutex                       m_Mutex;
		std::unordered_map<template_id_t, STemplate>    m_Templates;
		std::unordered_map<preset_id_t, SPreset>        m_Presets;
		std::unordered_map<std::string, template_id_t>  m_TemplateByName;
		std::unordered_map<std::string, preset_id_t>    m_PresetByName;
		template_id_t                                   m_NextTemplateId = 0;
		preset_id_t                                     m_NextPresetId = 0;
		CSignalCache* m_SignalRegistry = nullptr;
		CSignalScriptBridge* m_SignalBridge = nullptr;
	};
} // namespace pulvis::ecs