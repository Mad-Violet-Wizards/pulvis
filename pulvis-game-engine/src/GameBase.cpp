#include "GameBase.hpp"

#include "DomainRoots.hpp"
#include "MountSystem.hpp"
#include "assets/AssetRegistry.hpp"
#include "MessageBus.hpp"
#include "RenderSevice.hpp"
#include "LevelService.hpp"
#include "Logger.hpp"
#include "loaders/ShaderLoader.hpp"
#include "FileSourceDisk.hpp"
#include "EventDispatcher.hpp"
#include "ScriptableService.hpp"

//////////////////////////////////////////////////////////////////////////
namespace pulvis::game_engine
{
  CGameBase::CGameBase()
    : m_LastFrameTime(Clock::now())
    , m_MainChannelID(pulvis::threads::INVALID_CHANNEL_ID)
    , m_RenderChannelID(pulvis::threads::INVALID_CHANNEL_ID)
    , m_AudioChannelID(pulvis::threads::INVALID_CHANNEL_ID)
    , m_IOChannelID(pulvis::threads::INVALID_CHANNEL_ID)
    , m_DomainRoots(nullptr)
    , m_MountSystem(nullptr)
    , m_AssetRegistry(nullptr)
    , m_RenderService(nullptr)
    , m_LevelService(nullptr)
    , m_ScriptableService(nullptr)
    , m_MessageBus(nullptr)
    , m_EventDispatcher(nullptr)
  {

  }

  CGameBase::~CGameBase() = default;

  void CGameBase::Run()
  {
    // Derived class controls m_Config.
    Configure(m_Config);

    // Initialize engine systems.
    Initialize();

    // Do game specific initialization.
    OnInitialize();

    // Run the main loop.
    MainLoop();

    // Do game specific shutdown.
    OnShutdown();

    // Shutdown engine systems.
    Shutdown();
  }

  void CGameBase::Initialize()
  {
    PULVIS_INFO_LOG("Initializing core systems...");
    m_EventDispatcher = std::make_unique<pulvis::events::CEventDispatcher>();
    InitializeFilesystem();
    InitializeMessageBus();
    InitializeServices();
    PULVIS_INFO_LOG("Core systems initialized.");
  }

  void CGameBase::InitializeFilesystem()
  {
    m_DomainRoots = std::make_unique<pulvis::fs::CDomainRoots>(m_Config.AppName, m_Config.GameAssetsPath);
    m_MountSystem = std::make_unique<pulvis::fs::CMountSystem>();
    m_MountSystem->BootstrapDomains(*m_DomainRoots);

    // Let the game mount additional directories.
    InitializeGameFilesystems();

    m_AssetRegistry = std::make_unique<pulvis::fs::assets::CAssetRegistry>(*m_MountSystem);
  }

  void CGameBase::InitializeMessageBus()
  {
    m_MessageBus = std::make_unique<pulvis::threads::CMessageBus>();
    m_MainChannelID = m_MessageBus->RegisterChannel(4096);
    m_RenderChannelID = m_MessageBus->RegisterChannel(4096);
    m_AudioChannelID = m_MessageBus->RegisterChannel(4096);
    m_IOChannelID = m_MessageBus->RegisterChannel(65536);
  }

  void CGameBase::InitializeServices()
  {
		m_ScriptableService = std::make_unique<pulvis::scriptable::CScriptableService>();

    m_RenderService = std::make_unique<pulvis::rendering::CRenderService>(*m_AssetRegistry);
    m_RenderService->Initialize(m_Config.RendererType, m_Config.WindowWidth, m_Config.WindowHeight, m_Config.WindowTitle);

    m_LevelService = std::make_unique<pulvis::level::CLevelService>(*m_AssetRegistry, *m_MountSystem);
    m_LevelService->Initialize(*m_MessageBus, m_IOChannelID, m_MainChannelID);

    InitializeGameServices();
    m_ScriptableService->Initialize();
  }

  void CGameBase::Shutdown()
  {
    PULVIS_INFO_LOG("Shutting down core systems...");
    
    m_ScriptableService.reset();

    m_LevelService.reset();
    m_RenderService.reset();

    m_MessageBus.reset();
    m_AssetRegistry.reset();

    m_MountSystem.reset();
    m_DomainRoots.reset();

    PULVIS_INFO_LOG("Core systems shut down.");
  }

  void CGameBase::MainLoop()
  {
    m_LastFrameTime = Clock::now();

    while (!GetRenderService().GetShouldClose())
    {
      const Clock::time_point current_time = Clock::now();
      const float dt = std::chrono::duration<float>(current_time - m_LastFrameTime).count();
      m_LastFrameTime = current_time;

      m_MessageBus->Drain(m_MainChannelID);

      if (m_LevelService->HasActiveLevel())
      {
        m_LevelService->ApplyPendingChunks();
      }

      m_StateMachine.Frame(dt);
      Frame(dt);

      m_StateMachine.Render();
      Render();

      m_EventDispatcher->Frame(dt);

      m_RenderService->Frame();
    }
  }

  pulvis::fs::CDomainRoots& CGameBase::GetDomainRoots() const
  {
    ASSERT(m_DomainRoots, "DomainRoots not initialized.");
    return *m_DomainRoots;
  }

  pulvis::fs::CMountSystem& CGameBase::GetMountSystem() const
  {
    ASSERT(m_MountSystem, "MountSystem not initialized.");
    return *m_MountSystem;
  }

  pulvis::fs::assets::CAssetRegistry& CGameBase::GetAssetRegistry() const
  {
    ASSERT(m_AssetRegistry, "AssetRegistry not initialized.");
    return *m_AssetRegistry;
  }

  pulvis::rendering::CRenderService& CGameBase::GetRenderService() const
  {
    ASSERT(m_RenderService, "RenderService not initialized.");
    return *m_RenderService;
  }

  pulvis::level::CLevelService& CGameBase::GetLevelService() const
  {
    ASSERT(m_LevelService, "LevelService not initialized.");
    return *m_LevelService;
  }

  pulvis::scriptable::CScriptableService& CGameBase::GetScriptableService() const
  {
		ASSERT(m_ScriptableService, "ScriptableService not initialized.");
		return *m_ScriptableService;
  }

  pulvis::threads::CMessageBus& CGameBase::GetMessageBus() const
  {
    ASSERT(m_MessageBus, "MessageBus not initialized.");
    return *m_MessageBus;
  }

  pulvis::events::CEventDispatcher& CGameBase::GetEventDispatcher() const
  {
    ASSERT(m_EventDispatcher, "EventDispatcher not initialized.");
    return *m_EventDispatcher;
  }

  CGameStateMachine& CGameBase::GetStateMachine()
  {
    return m_StateMachine;
  }

  uint32_t CGameBase::GetMainChannelID() const
  {
    return m_MainChannelID;
  }

  uint32_t CGameBase::GetRenderChannelID() const
  {
    return m_RenderChannelID;
  }

  uint32_t CGameBase::GetAudioChannelID() const
  {
    return m_AudioChannelID;
  }

  uint32_t CGameBase::GetIOChannelID() const
  {
    return m_IOChannelID;
  }
}
