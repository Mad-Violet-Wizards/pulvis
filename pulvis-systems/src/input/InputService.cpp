#include "InputService.hpp"
#include "InputScriptableNode.hpp"
#include "InputEvents.hpp"

#include "EventDispatcher.hpp"
#include "ScriptableService.hpp"
#include "RenderService.hpp"
#include "opengl/GLRenderer.hpp"
#include "Window.hpp"
#include "Logger.hpp"
#include "Hash.hpp"
#include "assets/AssetRegistry.hpp"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <bit>

namespace pulvis::systems::input
{
  namespace
  {
    template <typename TArray>
    constexpr void BitSet(TArray& _bits, uint32_t _index)
    {
      _bits[_index >> 6u] |= (uint64_t{ 1 } << (_index & 63u));
    }

    template <typename TArray>
    constexpr void BitReset(TArray& _bits, uint32_t _index)
    {
      _bits[_index >> 6u] &= ~(uint64_t{ 1 } << (_index & 63u));
    }

    template <typename TArray>
    constexpr bool BitTest(const TArray& _bits, uint32_t _index)
    {
      return (_bits[_index >> 6u] & (uint64_t{ 1 } << (_index & 63u))) != 0;
    }

    template <typename TArray, typename Fn>
    void ForEachSetBit(const TArray& _bits, Fn&& _fn)
    {
      for (size_t w = 0; w < _bits.size(); ++w)
      {
        uint64_t v = _bits[w];
        while (v != 0)
        {
          const int bit = std::countr_zero(v);
          _fn(static_cast<uint32_t>(w * 64u + static_cast<uint32_t>(bit)));
          v &= v - 1;
        }
      }
    }

    template <typename TArray>
    TArray BitDiff(const TArray& _curr, const TArray& _prev)
    {
      TArray out{};
      for (size_t w = 0; w < out.size(); ++w)
      {
        out[w] = _curr[w] & ~_prev[w];
      }
      return out;
    }
  }

  CInputService::CInputService(pulvis::events::CEventDispatcher& _event_dispatcher,
    pulvis::scriptable::CScriptableService& _scriptable_service,
    pulvis::rendering::CRenderService& _render_service,
    pulvis::fs::assets::CAssetRegistry& _asset_registry)
    : m_EventDispatcher(_event_dispatcher)
    , m_ScriptableService(_scriptable_service)
    , m_RenderService(_render_service)
    , m_AssetRegistry(_asset_registry)
  {
  }

  CInputService::~CInputService() = default;

  void CInputService::Initialize()
  {
    if (m_Initialized) { return; }

    m_ScriptableNode = std::make_shared<CInputScriptableNode>(*this);
    m_ScriptableService.RegisterScriptableNode(m_ScriptableNode);

    auto* gl_renderer = static_cast<pulvis::rendering::gl::CGLRenderer*>(m_RenderService.GetRenderer());
    if (gl_renderer)
    {
      m_Window = gl_renderer->GetWindow().GetNativeWindow();
      if (m_Window)
      {
        InstallGlfwCallbacks(m_Window);
      }
    }

    m_Initialized = true;
    PULVIS_INFO_LOG("[Input] Service initialized.");
  }

  void CInputService::Shutdown()
  {
    if (!m_Initialized) { return; }

    if (m_Window)
    {
      UninstallGlfwCallbacks(m_Window);
      m_Window = nullptr;
    }

    m_BindingIndex.clear();
    m_HeldActions.clear();
    m_LastPressedTime.clear();
    m_Presets.clear();
    m_ActivePreset = nullptr;
    m_ActivePresetName.clear();
    m_ScriptableNode.reset();

    m_Initialized = false;
    PULVIS_INFO_LOG("[Input] Service shut down.");
  }

  void CInputService::InstallGlfwCallbacks(GLFWwindow* _window)
  {
    glfwSetWindowUserPointer(_window, this);

    m_PrevKeyCallback = glfwSetKeyCallback(_window, &CInputService::GlfwKeyCallback);
    m_PrevMouseCallback = glfwSetMouseButtonCallback(_window, &CInputService::GlfwMouseButtonCallback);
  }

  void CInputService::UninstallGlfwCallbacks(GLFWwindow* _window)
  {
    glfwSetKeyCallback(_window, m_PrevKeyCallback);
    glfwSetMouseButtonCallback(_window, m_PrevMouseCallback);
    m_PrevKeyCallback = nullptr;
    m_PrevMouseCallback = nullptr;
  }

  void CInputService::GlfwKeyCallback(GLFWwindow* _w, int _key, int _scancode, int _action, int _mods)
  {
    auto* self = static_cast<CInputService*>(glfwGetWindowUserPointer(_w));
    if (self && _key >= 0 && _key < static_cast<int>(MAX_KEYBOARD_KEYS))
    {
      if (_action == GLFW_PRESS)
      {
        BitSet(self->m_KeyboardCurr, static_cast<uint32_t>(_key));
        self->OnKeyEdge(static_cast<input_code_t>(_key), true);
      }
      else if (_action == GLFW_RELEASE)
      {
        BitReset(self->m_KeyboardCurr, static_cast<uint32_t>(_key));
        self->OnKeyEdge(static_cast<input_code_t>(_key), false);
      }
    }

    if (self && self->m_PrevKeyCallback)
    {
      self->m_PrevKeyCallback(_w, _key, _scancode, _action, _mods);
    }
  }

  void CInputService::GlfwMouseButtonCallback(GLFWwindow* _w, int _btn, int _action, int _mods)
  {
    auto* self = static_cast<CInputService*>(glfwGetWindowUserPointer(_w));
    if (self && _btn >= 0 && _btn < static_cast<int>(MAX_MOUSE_BUTTONS))
    {
      if (_action == GLFW_PRESS)
      {
        BitSet(self->m_MouseCurr, static_cast<uint32_t>(_btn));
        self->OnMouseEdge(static_cast<input_code_t>(_btn), true);
      }
      else if (_action == GLFW_RELEASE)
      {
        BitReset(self->m_MouseCurr, static_cast<uint32_t>(_btn));
        self->OnMouseEdge(static_cast<input_code_t>(_btn), false);
      }
    }

    if (self && self->m_PrevMouseCallback)
    {
      self->m_PrevMouseCallback(_w, _btn, _action, _mods);
    }
  }

  void CInputService::Frame(float _dt)
  {
    m_TimeAccumulator += _dt;

    PollGamepads();
    TickHeld(_dt);
  }

  void CInputService::LoadAndExecuteScripts()
  {
    constexpr const char* INPUT_DIR = "game/scripts/input";

    m_AssetRegistry.LoadDirectory(pulvis::fs::EDomain::Game, INPUT_DIR, true);

    const std::vector<pulvis::fs::assets::SAssetHandle> handles =
      m_AssetRegistry.EnumerateByPrefix(pulvis::fs::EAssetType::Script, INPUT_DIR);

    uint32_t executed = 0;
    for (const pulvis::fs::assets::SAssetHandle& handle : handles)
    {
      const pulvis::fs::assets::SAssetEntry* entry = m_AssetRegistry.Get(handle);
      if (entry == nullptr) { continue; }
      if (m_ScriptableService.ExecuteScriptByPath(entry->VirtualPath)) { ++executed; }
    }

    PULVIS_INFO_LOG("[Input] Executed {} preset script(s) under '{}'.", executed, INPUT_DIR);
  }

  void CInputService::PollGamepads()
  {
    for (uint32_t slot = 0; slot < MAX_GAMEPADS; ++slot)
    {
      m_GamepadPrev[slot] = m_GamepadCurr[slot];
      m_GamepadCurr[slot].fill(0);

      const int joy = GLFW_JOYSTICK_1 + static_cast<int>(slot);
      if (glfwJoystickPresent(joy) == GLFW_FALSE) { continue; }
      if (glfwJoystickIsGamepad(joy) == GLFW_FALSE) { continue; }

      GLFWgamepadstate gp{};
      if (glfwGetGamepadState(joy, &gp) == GLFW_FALSE) { continue; }

      for (int b = 0; b <= GLFW_GAMEPAD_BUTTON_LAST && b < static_cast<int>(MAX_GAMEPAD_BUTTONS); ++b)
      {
        if (gp.buttons[b] == GLFW_PRESS)
        {
          BitSet(m_GamepadCurr[slot], static_cast<uint32_t>(b));
        }
      }

      const auto pressed = BitDiff(m_GamepadCurr[slot], m_GamepadPrev[slot]);
      const auto released = BitDiff(m_GamepadPrev[slot], m_GamepadCurr[slot]);

      const int8_t s = static_cast<int8_t>(slot);
      ForEachSetBit(pressed, [this, s](uint32_t b) { OnGamepadEdge(static_cast<input_code_t>(b), s, true);  });
      ForEachSetBit(released, [this, s](uint32_t b) { OnGamepadEdge(static_cast<input_code_t>(b), s, false); });
    }
  }

  void CInputService::OnKeyEdge(input_code_t _key, bool _pressed)
  {
    if (!m_ActivePreset) { return; }

    const auto it = m_BindingIndex.find({ EInputDevice::Keyboard, _key });
    if (it == m_BindingIndex.end()) { return; }

    for (const SInputAction* action : it->second)
    {
      HandleEdge(*action, -1, _pressed);
    }
  }

  void CInputService::OnMouseEdge(input_code_t _btn, bool _pressed)
  {
    if (!m_ActivePreset) { return; }

    const auto it = m_BindingIndex.find({ EInputDevice::Mouse, _btn });
    if (it == m_BindingIndex.end()) { return; }

    for (const SInputAction* action : it->second)
    {
      HandleEdge(*action, -1, _pressed);
    }
  }

  void CInputService::OnGamepadEdge(input_code_t _btn, int8_t _slot, bool _pressed)
  {
    if (!m_ActivePreset) { return; }

    const auto it = m_BindingIndex.find({ EInputDevice::Gamepad, _btn });
    if (it == m_BindingIndex.end()) { return; }

    for (const SInputAction* action : it->second)
    {
      if (action->GamepadSlot >= 0 && action->GamepadSlot != _slot) { continue; }
      HandleEdge(*action, _slot, _pressed);
    }
  }

  void CInputService::HandleEdge(const SInputAction& _action, int8_t _slot, bool _pressed)
  {
    if (_pressed && !ModifiersSatisfied(_action.Modifiers)) { return; }

    switch (_action.Trigger)
    {
    case EInputTrigger::OnPressed:
    {
      if (!_pressed) { break; }
      if (HasFlag(_action.Modifiers, EInputModifierFlags::DoubleClick))
      {
        const float now = m_TimeAccumulator;
        auto        last_it = m_LastPressedTime.find(_action.Id);
        const float last = (last_it != m_LastPressedTime.end()) ? last_it->second : -1.0f;

        if (last >= 0.0f && (now - last) <= _action.DoubleClickWindow)
        {
          DispatchAction(_action, true);
          m_LastPressedTime.erase(_action.Id);
        }
        else
        {
          m_LastPressedTime[_action.Id] = now;
        }
      }
      else
      {
        DispatchAction(_action, true);
      }
      break;
    }
    case EInputTrigger::OnReleased:
    {
      if (!_pressed) { DispatchAction(_action, false); }
      break;
    }
    case EInputTrigger::OnHeld:
    {
      if (_pressed)
      {
        SHeldRuntime rt{};
        rt.Action = &_action;
        rt.GamepadSlot = _slot;
        rt.HoldElapsed = 0.0f;
        rt.HoldFired = false;

        const float threshold = HasFlag(_action.Modifiers, EInputModifierFlags::HoldTime)
          ? _action.HoldTime
          : 0.0f;
        if (threshold <= 0.0f)
        {
          DispatchAction(_action, true);
          rt.HoldFired = true;
        }
        m_HeldActions.push_back(rt);
      }
      else
      {
        // Fire a symmetric release event for any OnHeld action that previously
        // dispatched with Pressed=true. Lets event-driven consumers (movement,
        // aim, charged abilities) zero out their state on key-up.
        for (auto it = m_HeldActions.begin(); it != m_HeldActions.end(); )
        {
          if (it->Action->Id == _action.Id && it->GamepadSlot == _slot)
          {
            if (it->HoldFired) { DispatchAction(*it->Action, false); }
            it = m_HeldActions.erase(it);
          }
          else
          {
            ++it;
          }
        }
      }
      break;
    }
    case EInputTrigger::OnDoubleClick:
    {
      if (!_pressed) { break; }

      const float now = m_TimeAccumulator;
      auto        last_it = m_LastPressedTime.find(_action.Id);
      const float last = (last_it != m_LastPressedTime.end()) ? last_it->second : -1.0f;

      if (last >= 0.0f && (now - last) <= _action.DoubleClickWindow)
      {
        DispatchAction(_action, true);
        m_LastPressedTime.erase(_action.Id);
      }
      else
      {
        m_LastPressedTime[_action.Id] = now;
      }
      break;
    }
    }
  }

  void CInputService::TickHeld(float _dt)
  {
    for (SHeldRuntime& rt : m_HeldActions)
    {
      if (rt.HoldFired) { continue; }

      rt.HoldElapsed += _dt;
      const float threshold = HasFlag(rt.Action->Modifiers, EInputModifierFlags::HoldTime)
        ? rt.Action->HoldTime
        : 0.0f;
      if (rt.HoldElapsed >= threshold)
      {
        DispatchAction(*rt.Action, true);
        rt.HoldFired = true;
      }
    }
  }

  bool CInputService::ModifiersSatisfied(EInputModifierFlags _flags) const
  {
    const bool needs_shift = HasFlag(_flags, EInputModifierFlags::Shift);
    const bool needs_ctrl = HasFlag(_flags, EInputModifierFlags::Ctrl);
    const bool needs_alt = HasFlag(_flags, EInputModifierFlags::Alt);
    const bool needs_super = HasFlag(_flags, EInputModifierFlags::Super);

    if (needs_shift && !(BitTest(m_KeyboardCurr, GLFW_KEY_LEFT_SHIFT) || BitTest(m_KeyboardCurr, GLFW_KEY_RIGHT_SHIFT))) { return false; }
    if (needs_ctrl && !(BitTest(m_KeyboardCurr, GLFW_KEY_LEFT_CONTROL) || BitTest(m_KeyboardCurr, GLFW_KEY_RIGHT_CONTROL))) { return false; }
    if (needs_alt && !(BitTest(m_KeyboardCurr, GLFW_KEY_LEFT_ALT) || BitTest(m_KeyboardCurr, GLFW_KEY_RIGHT_ALT))) { return false; }
    if (needs_super && !(BitTest(m_KeyboardCurr, GLFW_KEY_LEFT_SUPER) || BitTest(m_KeyboardCurr, GLFW_KEY_RIGHT_SUPER))) { return false; }

    return true;
  }

  void CInputService::RegisterPreset(std::string _name, std::vector<SInputAction> _actions)
  {
    auto preset = std::make_unique<CInputPreset>(_name);
    for (SInputAction& a : _actions)
    {
      a.Id = pulvis::tl::hash::fnv1a<action_id_t>(a.Name.c_str());
      preset->AddAction(std::move(a));
    }

    const std::string key = _name;
    m_Presets[key] = std::move(preset);

    if (!m_ActivePreset)
    {
      SwitchPreset(key);
    }
  }

  bool CInputService::SwitchPreset(std::string_view _preset_name)
  {
    const auto it = m_Presets.find(std::string(_preset_name));
    if (it == m_Presets.end())
    {
      PULVIS_WARNING_LOG("[Input] Cannot switch to unknown preset '{}'.", _preset_name);
      return false;
    }

    m_ActivePreset = it->second.get();
    m_ActivePresetName = it->first;
    m_HeldActions.clear();
    m_LastPressedTime.clear();
    RebuildBindingIndex();

    m_EventDispatcher.Submit<SInputPresetChangedEvent>(m_ActivePresetName);
    PULVIS_INFO_LOG("[Input] Active preset switched to '{}'.", m_ActivePresetName);
    return true;
  }

  bool CInputService::HasPreset(std::string_view _name) const
  {
    return m_Presets.find(std::string(_name)) != m_Presets.end();
  }

  void CInputService::RebuildBindingIndex()
  {
    m_BindingIndex.clear();
    if (!m_ActivePreset) { return; }

    for (const SInputAction& action : m_ActivePreset->GetActions())
    {
      m_BindingIndex[{ action.Device, action.Code }].push_back(&action);
    }
  }

  void CInputService::DispatchAction(const SInputAction& _action, bool _pressed)
  {
    m_EventDispatcher.Submit<SInputActionEvent>(_action.Name, _action.Id, _action.Trigger, _pressed);

    sol::state& lua = m_ScriptableService.GetLuaState();
    sol::protected_function on_action = lua["OnInputAction"];
    if (on_action.valid())
    {
      sol::protected_function_result result = on_action(_action.Name, _pressed);
      if (!result.valid())
      {
        sol::error err = result;
        PULVIS_ERROR_LOG("[Input] OnInputAction('{}') failed: {}", _action.Name, err.what());
      }
    }
  }
} // namespace pulvis::systems::input