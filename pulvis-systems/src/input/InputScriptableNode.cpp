#include "InputScriptableNode.hpp"
#include "InputService.hpp"
#include "InputBindings.hpp"
#include "Logger.hpp"

#include <GLFW/glfw3.h>

#include <vector>

namespace pulvis::systems::input
{
  namespace
  {
    // Builder usertype that lives only on the Lua stack during Preset(...).
    struct SPresetBuilder final
    {
      std::vector<SInputAction> Actions;
    };

    [[nodiscard]] EInputDevice DeviceFromCode(input_code_t _code, sol::optional<int> _explicit_device)
    {
      if (_explicit_device) { return static_cast<EInputDevice>(*_explicit_device); }

      // Mouse buttons share low ids with keyboard - prefer keyboard unless
      // explicitly overridden via { Device = Device_Mouse }.
      if (_code >= GLFW_KEY_SPACE && _code <= GLFW_KEY_LAST) { return EInputDevice::Keyboard; }
      if (_code >= GLFW_MOUSE_BUTTON_1 && _code <= GLFW_MOUSE_BUTTON_LAST) { return EInputDevice::Mouse; }
      return EInputDevice::Keyboard;
    }

    void RegisterEnums(sol::state& _lua)
    {
      // Devices
      _lua["Device_Keyboard"] = static_cast<int>(EInputDevice::Keyboard);
      _lua["Device_Mouse"] = static_cast<int>(EInputDevice::Mouse);
      _lua["Device_Gamepad"] = static_cast<int>(EInputDevice::Gamepad);

      // Triggers (short form)
      _lua["OnPress"] = static_cast<int>(EInputTrigger::OnPressed);
      _lua["OnRelease"] = static_cast<int>(EInputTrigger::OnReleased);
      _lua["OnHold"] = static_cast<int>(EInputTrigger::OnHeld);
      _lua["OnDoubleClick"] = static_cast<int>(EInputTrigger::OnDoubleClick);

      // Modifier flags
      _lua["Mod_None"] = static_cast<int>(EInputModifierFlags::None);
      _lua["Mod_Shift"] = static_cast<int>(EInputModifierFlags::Shift);
      _lua["Mod_Ctrl"] = static_cast<int>(EInputModifierFlags::Ctrl);
      _lua["Mod_Alt"] = static_cast<int>(EInputModifierFlags::Alt);
      _lua["Mod_Super"] = static_cast<int>(EInputModifierFlags::Super);
      _lua["Mod_HoldTime"] = static_cast<int>(EInputModifierFlags::HoldTime);
      _lua["Mod_DoubleClick"] = static_cast<int>(EInputModifierFlags::DoubleClick);

      // Letter keys
      _lua["Key_A"] = GLFW_KEY_A; _lua["Key_B"] = GLFW_KEY_B; _lua["Key_C"] = GLFW_KEY_C;
      _lua["Key_D"] = GLFW_KEY_D; _lua["Key_E"] = GLFW_KEY_E; _lua["Key_F"] = GLFW_KEY_F;
      _lua["Key_G"] = GLFW_KEY_G; _lua["Key_H"] = GLFW_KEY_H; _lua["Key_I"] = GLFW_KEY_I;
      _lua["Key_J"] = GLFW_KEY_J; _lua["Key_K"] = GLFW_KEY_K; _lua["Key_L"] = GLFW_KEY_L;
      _lua["Key_M"] = GLFW_KEY_M; _lua["Key_N"] = GLFW_KEY_N; _lua["Key_O"] = GLFW_KEY_O;
      _lua["Key_P"] = GLFW_KEY_P; _lua["Key_Q"] = GLFW_KEY_Q; _lua["Key_R"] = GLFW_KEY_R;
      _lua["Key_S"] = GLFW_KEY_S; _lua["Key_T"] = GLFW_KEY_T; _lua["Key_U"] = GLFW_KEY_U;
      _lua["Key_V"] = GLFW_KEY_V; _lua["Key_W"] = GLFW_KEY_W; _lua["Key_X"] = GLFW_KEY_X;
      _lua["Key_Y"] = GLFW_KEY_Y; _lua["Key_Z"] = GLFW_KEY_Z;

      _lua["Key_Space"] = GLFW_KEY_SPACE;
      _lua["Key_Enter"] = GLFW_KEY_ENTER;
      _lua["Key_Escape"] = GLFW_KEY_ESCAPE;
      _lua["Key_Tab"] = GLFW_KEY_TAB;
      _lua["Key_Up"] = GLFW_KEY_UP;
      _lua["Key_Down"] = GLFW_KEY_DOWN;
      _lua["Key_Left"] = GLFW_KEY_LEFT;
      _lua["Key_Right"] = GLFW_KEY_RIGHT;

      // Mouse
      _lua["Mouse_Left"] = GLFW_MOUSE_BUTTON_LEFT;
      _lua["Mouse_Right"] = GLFW_MOUSE_BUTTON_RIGHT;
      _lua["Mouse_Middle"] = GLFW_MOUSE_BUTTON_MIDDLE;

      // Gamepad (GLFW unified mapping; works for XInput, DualShock, Switch Pro)
      _lua["Pad_A"] = GLFW_GAMEPAD_BUTTON_A;
      _lua["Pad_B"] = GLFW_GAMEPAD_BUTTON_B;
      _lua["Pad_X"] = GLFW_GAMEPAD_BUTTON_X;
      _lua["Pad_Y"] = GLFW_GAMEPAD_BUTTON_Y;
      _lua["Pad_LeftBumper"] = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
      _lua["Pad_RightBumper"] = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
      _lua["Pad_Back"] = GLFW_GAMEPAD_BUTTON_BACK;
      _lua["Pad_Start"] = GLFW_GAMEPAD_BUTTON_START;
      _lua["Pad_DPadUp"] = GLFW_GAMEPAD_BUTTON_DPAD_UP;
      _lua["Pad_DPadDown"] = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
      _lua["Pad_DPadLeft"] = GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
      _lua["Pad_DPadRight"] = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
    }
  }

  CInputScriptableNode::CInputScriptableNode(CInputService& _service)
    : m_Service(_service)
  {
  }

  void CInputScriptableNode::RegisterLuaBindings(sol::state& _luaState)
  {
    RegisterEnums(_luaState);

    _luaState.new_usertype<SPresetBuilder>("PresetBuilder",
      "Action",
      [](SPresetBuilder& _self,
        const std::string& _name,
        input_code_t _code,
        int _trigger,
        sol::optional<sol::table> _modifiers)
      {
        SInputAction action{};
        action.Name = _name;
        action.Code = _code;
        action.Trigger = static_cast<EInputTrigger>(_trigger);

        sol::optional<int> explicit_device;
        if (_modifiers)
        {
          sol::table tbl = *_modifiers;

          if (auto flags = tbl.get<sol::optional<int>>("Flags"))
          {
            action.Modifiers = static_cast<EInputModifierFlags>(*flags);
          }
          if (auto ht = tbl.get<sol::optional<float>>("HoldTime"))
          {
            action.HoldTime = *ht;
            action.Modifiers = action.Modifiers | EInputModifierFlags::HoldTime;
          }
          if (auto dcw = tbl.get<sol::optional<float>>("DoubleClickWindow"))
          {
            action.DoubleClickWindow = *dcw;
          }
          if (auto dev = tbl.get<sol::optional<int>>("Device"))
          {
            explicit_device = dev;
          }
          if (auto slot = tbl.get<sol::optional<int>>("GamepadSlot"))
          {
            action.GamepadSlot = static_cast<int8_t>(*slot);
          }
        }

        action.Device = DeviceFromCode(action.Code, explicit_device);
        _self.Actions.push_back(std::move(action));
      });

    _luaState.set_function("Preset",
      [this](const std::string& _name, sol::protected_function _body)
      {
        SPresetBuilder builder;
        sol::protected_function_result result = _body(std::ref(builder));
        if (!result.valid())
        {
          sol::error err = result;
          PULVIS_ERROR_LOG("[Input] Preset('{}') body failed: {}", _name, err.what());
          return;
        }
        m_Service.RegisterPreset(_name, std::move(builder.Actions));
      });

    _luaState.set_function("SwitchPreset",
      [this](const std::string& _name)
      {
        return m_Service.SwitchPreset(_name);
      });
  }
} // namespace pulvis::systems::input