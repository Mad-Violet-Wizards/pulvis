#pragma once

#include "InputBindings.hpp"
#include "InputTypes.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct GLFWwindow;
typedef void (*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
typedef void (*GLFWmousebuttonfun)(GLFWwindow*, int, int, int);

namespace pulvis::events { class CEventDispatcher; }
namespace pulvis::scriptable { class CScriptableService; }
namespace pulvis::rendering { class CRenderService; }
namespace pulvis::fs::assets { class CAssetRegistry; }

namespace pulvis::systems::input
{
  class CInputScriptableNode;

  class CInputService final
  {
  public:

    CInputService(pulvis::events::CEventDispatcher& _event_dispatcher,
      pulvis::scriptable::CScriptableService& _scriptable_service,
      pulvis::rendering::CRenderService& _render_service,
      pulvis::fs::assets::CAssetRegistry& _asset_registry);
    ~CInputService();

    CInputService(const CInputService&) = delete;
    CInputService& operator=(const CInputService&) = delete;

    void Initialize();
    void Shutdown();

    // Per-frame: gamepad poll + held-action ticking. Edge events
    // for keyboard/mouse arrive via GLFW callbacks installed in Initialize().
    void Frame(float _dt);

    void LoadAndExecuteScripts();

    void RegisterPreset(std::string _name, std::vector<SInputAction> _actions);

    // Switch the active preset by name. Returns false if not found.
    bool SwitchPreset(std::string_view _preset_name);

    [[nodiscard]] std::string_view GetActivePresetName() const { return m_ActivePresetName; }
    [[nodiscard]] bool             HasPreset(std::string_view _name) const;

    [[nodiscard]] std::shared_ptr<CInputScriptableNode> GetScriptableNode() const { return m_ScriptableNode; }

  private:

    struct SBindingKey final
    {
      EInputDevice Device;
      input_code_t Code;

      bool operator==(const SBindingKey& _o) const noexcept
      {
        return Device == _o.Device && Code == _o.Code;
      }
    };

    struct SBindingKeyHash final
    {
      size_t operator()(const SBindingKey& _k) const noexcept
      {
        return (static_cast<size_t>(_k.Code) * 31u) ^ static_cast<size_t>(_k.Device);
      }
    };

    struct SHeldRuntime final
    {
      const SInputAction* Action = nullptr;
      int8_t              GamepadSlot = -1;
      float               HoldElapsed = 0.0f;
      bool                HoldFired = false;
    };

    void InstallGlfwCallbacks(GLFWwindow* _window);
    void UninstallGlfwCallbacks(GLFWwindow* _window);
    void RebuildBindingIndex();

    void OnKeyEdge(input_code_t _key, bool _pressed);
    void OnMouseEdge(input_code_t _btn, bool _pressed);
    void OnGamepadEdge(input_code_t _btn, int8_t _slot, bool _pressed);

    void HandleEdge(const SInputAction& _action, int8_t _slot, bool _pressed);
    void DispatchAction(const SInputAction& _action, bool _pressed);

    [[nodiscard]] bool ModifiersSatisfied(EInputModifierFlags _flags) const;

    void PollGamepads();
    void TickHeld(float _dt);

    static void GlfwKeyCallback(GLFWwindow* _w, int _key, int _scancode, int _action, int _mods);
    static void GlfwMouseButtonCallback(GLFWwindow* _w, int _btn, int _action, int _mods);

    pulvis::events::CEventDispatcher& m_EventDispatcher;
    pulvis::scriptable::CScriptableService& m_ScriptableService;
    pulvis::rendering::CRenderService& m_RenderService;
    pulvis::fs::assets::CAssetRegistry& m_AssetRegistry;

    std::shared_ptr<CInputScriptableNode>    m_ScriptableNode;

    std::unordered_map<std::string, std::unique_ptr<CInputPreset>> m_Presets;
    std::string                                                    m_ActivePresetName;
    CInputPreset* m_ActivePreset = nullptr;

    std::unordered_map<SBindingKey, std::vector<const SInputAction*>, SBindingKeyHash> m_BindingIndex;

    keyboard_bits_t m_KeyboardCurr{};
    mouse_bits_t    m_MouseCurr{};
    gamepad_bits_t  m_GamepadCurr[MAX_GAMEPADS]{};
    gamepad_bits_t  m_GamepadPrev[MAX_GAMEPADS]{};

    std::vector<SHeldRuntime>              m_HeldActions;
    std::unordered_map<action_id_t, float> m_LastPressedTime;

    GLFWwindow* m_Window = nullptr;
    GLFWkeyfun         m_PrevKeyCallback = nullptr;
    GLFWmousebuttonfun m_PrevMouseCallback = nullptr;

    float m_TimeAccumulator = 0.0f;
    bool  m_Initialized = false;
  };
} // namespace pulvis::systems::input