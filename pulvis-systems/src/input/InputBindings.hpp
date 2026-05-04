#pragma once

#include "InputTypes.hpp"

#include <string>
#include <vector>
#include <cstdint>

namespace pulvis::systems::input
{
  struct SInputAction final
  {
    std::string         Name;
    action_id_t         Id = 0;
    EInputDevice        Device = EInputDevice::Keyboard;
    input_code_t        Code = 0;
    EInputTrigger       Trigger = EInputTrigger::OnPressed;
    EInputModifierFlags Modifiers = EInputModifierFlags::None;

    // Modifier parameters - only consulted when the matching flag is set.
    float               HoldTime = 0.0f;
    float               DoubleClickWindow = 0.25f;

    // Optional gamepad slot (0..MAX_GAMEPADS-1); -1 = any.
    int8_t              GamepadSlot = -1;
  };

  class CInputPreset final
  {
  public:

    explicit CInputPreset(std::string _name) : m_Name(std::move(_name)) {}

    void AddAction(SInputAction _action) { m_Actions.push_back(std::move(_action)); }

    [[nodiscard]] const std::string& GetName()    const { return m_Name; }
    [[nodiscard]] const std::vector<SInputAction>& GetActions() const { return m_Actions; }

  private:

    std::string                 m_Name;
    std::vector<SInputAction>   m_Actions;
  };
} // namespace pulvis::systems::input