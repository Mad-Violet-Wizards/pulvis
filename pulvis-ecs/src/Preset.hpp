#pragma once

#include "ComponentBlueprint.hpp"

#include <vector>

namespace pulvis::ecs
{
  struct SPresetFieldOverride final
  {
    component_id_t ComponentId = INVALID_COMPONENT_ID;
    std::string    FieldName;
    std::any       Value;
  };

  struct SPreset final
  {
    preset_id_t                       Id = INVALID_PRESET_ID;
    template_id_t                     BaseTemplate = INVALID_TEMPLATE_ID;
    std::string                       Name;
    std::vector<SPresetFieldOverride> Overrides;
  };
}