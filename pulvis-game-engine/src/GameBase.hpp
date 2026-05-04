#pragma once

#include "IGame.hpp"
#include "EngineConfig.hpp"
#include "GameStateMachine.hpp"
#include "input/InputService.hpp"
#include <memory>
#include <chrono>

///////////////////////////////////////////////////////////////////////////////////////////////////
namespace pulvis::fs
{
	class CDomainRoots;
	class CMountSystem;

	namespace assets
	{
		class CAssetRegistry;
	}
}

namespace pulvis::rendering
{
	class CRenderService;
}

namespace pulvis::level
{
	class CLevelService;
}

namespace pulvis::threads
{
	class CMessageBus;
}

namespace pulvis::events
{
  class CEventDispatcher;
  class CEventScriptBridge;
}

namespace pulvis::scriptable
{
	class CScriptableService;
}

namespace pulvis::ecs
{
  class CEcsService;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

namespace pulvis::game_engine
{
  class CGameBase : public IGame
  {
  public:

    CGameBase();
    virtual ~CGameBase();

    CGameBase(const CGameBase&) = delete;
    CGameBase& operator=(const CGameBase&) = delete;

    void Run();

  public:

    [[nodiscard]] pulvis::fs::CDomainRoots& GetDomainRoots() const;
    [[nodiscard]] pulvis::fs::CMountSystem& GetMountSystem() const;
    [[nodiscard]] pulvis::fs::assets::CAssetRegistry& GetAssetRegistry() const;
    [[nodiscard]] pulvis::rendering::CRenderService& GetRenderService() const;
    [[nodiscard]] pulvis::level::CLevelService& GetLevelService() const;
	  [[nodiscard]] pulvis::scriptable::CScriptableService& GetScriptableService() const;
    [[nodiscard]] pulvis::ecs::CEcsService& GetEcsService() const;
    [[nodiscard]] pulvis::threads::CMessageBus& GetMessageBus() const;
    [[nodiscard]] pulvis::events::CEventDispatcher& GetEventDispatcher() const;
		[[nodiscard]] pulvis::events::CEventScriptBridge& GetEventScriptBridge() const;
    [[nodiscard]] pulvis::systems::input::CInputService& GetInputService() const;

    CGameStateMachine& GetStateMachine();

  public:

    uint32_t GetMainChannelID() const;
    uint32_t GetRenderChannelID() const;
    uint32_t GetAudioChannelID() const;
    uint32_t GetIOChannelID() const;

  private:

    void Initialize();
    void InitializeFilesystem();
    void InitializeMessageBus();
    void InitializeServices();

    void Shutdown();
    void MainLoop();

  private:

    SEngineConfig m_Config;

    std::unique_ptr<pulvis::fs::CDomainRoots> m_DomainRoots;
    std::unique_ptr<pulvis::fs::CMountSystem> m_MountSystem;
    std::unique_ptr<pulvis::fs::assets::CAssetRegistry> m_AssetRegistry;
    std::unique_ptr<pulvis::rendering::CRenderService> m_RenderService;
    std::unique_ptr<pulvis::level::CLevelService> m_LevelService;
		std::unique_ptr<pulvis::scriptable::CScriptableService> m_ScriptableService;
		std::unique_ptr<pulvis::ecs::CEcsService> m_EcsService;
    std::unique_ptr<pulvis::threads::CMessageBus> m_MessageBus;
    std::unique_ptr<pulvis::events::CEventDispatcher> m_EventDispatcher;
		std::unique_ptr<pulvis::events::CEventScriptBridge> m_EventScriptBridge;
		std::unique_ptr<pulvis::systems::input::CInputService> m_InputService;

    CGameStateMachine m_StateMachine;

    // Cached MessageBus channel IDS.
    uint32_t m_MainChannelID;
    uint32_t m_RenderChannelID;
    uint32_t m_AudioChannelID;
    uint32_t m_IOChannelID;


    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point m_LastFrameTime;
  };
}
