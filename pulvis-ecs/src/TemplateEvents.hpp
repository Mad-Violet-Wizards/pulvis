#pragma once

#include "Event.hpp"
#include "EcsTypes.hpp"

#include <string>

namespace pulvis::ecs
{
	struct STemplateRegisteredEvent : public pulvis::events::TEvent<STemplateRegisteredEvent>
	{
		template_id_t TemplateId = INVALID_TEMPLATE_ID;
		std::string Name;
	};

	struct SPresetRegisteredEvent : public pulvis::events::TEvent<SPresetRegisteredEvent>
	{
		preset_id_t PresetId = INVALID_PRESET_ID;
		std::string Name;
	};
}