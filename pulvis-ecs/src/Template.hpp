#pragma once

#include "ComponentBlueprint.hpp"
#include "ComponentTraits.hpp"

#include <vector>
#include <string>
#include <unordered_map>

namespace pulvis::ecs
{
	//////////////////////////////////////////////////////////////////////////
	struct SComponentHookOverride final
	{
		SLifecycleHooks LifecycleHooks;
		CHookRegistry   CustomHooks;
	};

	//////////////////////////////////////////////////////////////////////////
	struct STemplateSignalBinding final
	{
		component_id_t SourceComponent = INVALID_COMPONENT_ID;
		std::string    SignalName;
		std::string    TargetScriptHandler;
	};

	//////////////////////////////////////////////////////////////////////////
	struct STemplate final
	{
		STemplate() = default;
		STemplate(const STemplate&) = default;
		STemplate& operator=(const STemplate&) = default;
		STemplate(STemplate&&) noexcept = default;
		STemplate& operator=(STemplate&&) noexcept = default;

		template_id_t                                              Id = INVALID_TEMPLATE_ID;
		std::string                                                Name;
		std::string                                                ScriptPath;
		std::vector<SComponentBlueprint>                           Components;
		std::unordered_map<component_id_t, SComponentHookOverride> HookOverrides;
		std::vector<STemplateSignalBinding>                        SignalBindings;
	};
} // namespace pulvis::ecs