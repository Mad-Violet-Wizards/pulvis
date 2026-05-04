#pragma once

#include <array>
#include <cstdint>

namespace pulvis::systems::input
{
  using action_id_t = uint32_t;
  using input_code_t = int32_t;

  enum class EInputDevice : uint8_t
  {
    Keyboard = 0,
    Mouse = 1,
    Gamepad = 2
  };

  enum class EInputTrigger : uint8_t
  {
    OnPressed = 0,
    OnReleased = 1,
    OnHeld = 2,
    OnDoubleClick = 3
  };

  enum class EInputModifierFlags : uint16_t
  {
    None = 0,
    Shift = 1u << 0,
    Ctrl = 1u << 1,
    Alt = 1u << 2,
    Super = 1u << 3,
    HoldTime = 1u << 4,
    DoubleClick = 1u << 5
  };

  [[nodiscard]] constexpr EInputModifierFlags operator|(EInputModifierFlags _a, EInputModifierFlags _b)
  {
    return static_cast<EInputModifierFlags>(static_cast<uint16_t>(_a) | static_cast<uint16_t>(_b));
  }

  [[nodiscard]] constexpr EInputModifierFlags operator&(EInputModifierFlags _a, EInputModifierFlags _b)
  {
    return static_cast<EInputModifierFlags>(static_cast<uint16_t>(_a) & static_cast<uint16_t>(_b));
  }

  [[nodiscard]] constexpr bool HasFlag(EInputModifierFlags _value, EInputModifierFlags _flag)
  {
    return (static_cast<uint16_t>(_value) & static_cast<uint16_t>(_flag)) != 0;
  }

  constexpr uint32_t MAX_KEYBOARD_KEYS = 384;
  constexpr uint32_t MAX_MOUSE_BUTTONS = 8;
  constexpr uint32_t MAX_GAMEPAD_BUTTONS = 32;
  constexpr uint32_t MAX_GAMEPADS = 1;

  constexpr uint32_t KEYBOARD_WORDS = (MAX_KEYBOARD_KEYS + 63u) / 64u;
  constexpr uint32_t MOUSE_WORDS = (MAX_MOUSE_BUTTONS + 63u) / 64u;
  constexpr uint32_t GAMEPAD_WORDS = (MAX_GAMEPAD_BUTTONS + 63u) / 64u;

  using keyboard_bits_t = std::array<uint64_t, KEYBOARD_WORDS>;
  using mouse_bits_t = std::array<uint64_t, MOUSE_WORDS>;
  using gamepad_bits_t = std::array<uint64_t, GAMEPAD_WORDS>;
} // namespace pulvis::systems::input