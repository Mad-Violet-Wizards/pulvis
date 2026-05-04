#pragma once

#include "Event.hpp"
#include "InputTypes.hpp"

#include <string>

namespace pulvis::systems::input
{
  /*
    Carries one dispatched input action. Pressed describes the underlying
    edge state of this dispatch:
      - OnPressed       -> Pressed = true
      - OnReleased      -> Pressed = false
      - OnDoubleClick   -> Pressed = true
      - OnHeld          -> Pressed = true on press (or when HoldTime threshold
                            is reached), Pressed = false on release of an
                            action that previously fired with Pressed = true.

    Consumers that need symmetric press/release semantics (movement,
    aim, charge attacks letting go) should branch on Pressed.
  */
  struct SInputActionEvent final : public pulvis::events::TEvent<SInputActionEvent>
  {
    SInputActionEvent(std::string _action_name, action_id_t _action_id, EInputTrigger _trigger, bool _pressed)
      : ActionName(std::move(_action_name))
      , ActionId(_action_id)
      , Trigger(_trigger)
      , Pressed(_pressed)
    {
    }

    std::string   ActionName;
    action_id_t   ActionId;
    EInputTrigger Trigger;
    bool          Pressed;
  };

  struct SInputPresetChangedEvent final : public pulvis::events::TEvent<SInputPresetChangedEvent>
  {
    explicit SInputPresetChangedEvent(std::string _preset_name)
      : PresetName(std::move(_preset_name))
    {
    }

    std::string PresetName;
  };
} // namespace pulvis::systems::input