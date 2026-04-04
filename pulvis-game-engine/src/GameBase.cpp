#include "GameBase.hpp"

#include "DomainRoots.hpp"
#include "MountSystem.hpp"
#include "assets/AssetRegistry.hpp"
#include "MessageBus.hpp"
#include "RenderSevice.hpp"
#include "LevelService.hpp"
#include "Logger.hpp"
#include "loaders/ShaderLoader.hpp"

//////////////////////////////////////////////////////////////////////////
namespace
{
  enum EChannel : uint32_t
  {
    Main = 0,
    Render = 1,
    Audio = 2,
  };

  struct SWindowResized
  {
    int Width;
    int Height;
  };

  struct SAudioPlayCmd
  {
    uint32_t SoundId;
    float Volume;
  };

  struct SLoadLevelCmd
  {
    uint32_t LevelId;
  };
}
//////////////////////////////////////////////////////////////////////////

namespace pulvis::game_engine
{
    CGameBase::CGameBase()
        : m_LastFrameTime(Clock::now())
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
				InitializeFilesystem();
				InitializeMessageBus();
				InitializeServices();
        PULVIS_INFO_LOG("Core systems initialized.");
    }

    void CGameBase::InitializeFilesystem()
    {
      m_DomainRoots = std::make_unique<pulvis::fs::CDomainRoots>(m_Config.AppName);
      m_MountSystem = std::make_unique<pulvis::fs::CMountSystem>();
      m_MountSystem->BootstrapDomains(*m_DomainRoots);

      m_AssetRegistry = std::make_unique<pulvis::fs::assets::CAssetRegistry>(*m_MountSystem);

    }

    void CGameBase::InitializeMessageBus()
    {
      m_MessageBus = std::make_unique<pulvis::threads::CMessageBus>();
      m_MessageBus->RegisterChannel(EChannel::Main, 4096);
      m_MessageBus->RegisterChannel(EChannel::Render, 4096);
      m_MessageBus->RegisterChannel(EChannel::Audio, 4096);

      int resize_count = 0;
      int* rc = &resize_count;

      m_MessageBus->RegisterHandler<SWindowResized>(EChannel::Main,
        [rc](const SWindowResized& _msg) {
          *rc += 1;
          PULVIS_INFO_LOG("[Main] WindowResized: {}x{}", _msg.Width, _msg.Height);
        }
      );

      m_MessageBus->RegisterHandler<SAudioPlayCmd>(EChannel::Audio,
        +[](const SAudioPlayCmd& _msg) {
          PULVIS_INFO_LOG("[Audio] Play sound {} at volume {:.2f}", _msg.SoundId, _msg.Volume);
        }
      );

      m_MessageBus->RegisterHandler<SLoadLevelCmd>(EChannel::Main,
        +[](const SLoadLevelCmd& _msg) {
          PULVIS_INFO_LOG("[Main] Load level {}", _msg.LevelId);
        }
      );


      m_MessageBus->Send<SWindowResized>(EChannel::Main, SWindowResized{ 1920, 1080 });
      m_MessageBus->Send<SWindowResized>(EChannel::Main, SWindowResized{ 2560, 1440 });
      m_MessageBus->Send<SAudioPlayCmd>(EChannel::Audio, SAudioPlayCmd{ 42, 0.8f });
      m_MessageBus->Send<SLoadLevelCmd>(EChannel::Main, SLoadLevelCmd{ 1 });

      m_MessageBus->Send<SAudioPlayCmd>(EChannel::Render, SAudioPlayCmd{ 99, 1.0f });

      PULVIS_INFO_LOG("Draining Main channel");
      m_MessageBus->Drain(EChannel::Main);

      PULVIS_INFO_LOG("Draining Audio channel");
      m_MessageBus->Drain(EChannel::Audio);

      PULVIS_INFO_LOG("Draining Render channel (no handlers)");
      m_MessageBus->Drain(EChannel::Render);

      PULVIS_INFO_LOG("MessageBus smoke test: resize_count = {} (expected 2)", resize_count);

    }

    void CGameBase::InitializeServices()
    {
      m_RenderService = std::make_unique<pulvis::rendering::CRenderService>(*m_AssetRegistry);
      m_RenderService->Initialize(m_Config.RendererType, m_Config.WindowWidth, m_Config.WindowHeight, m_Config.WindowTitle);
      m_LevelService = std::make_unique<pulvis::level::CLevelService>(*m_AssetRegistry, *m_MountSystem);
    }

    void CGameBase::Shutdown()
    {
        PULVIS_INFO_LOG("Shutting down core systems...");

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

            m_StateMachine.Frame(*this, dt);
            Frame(dt);

            m_StateMachine.Render(*this);
            Render();

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

    pulvis::threads::CMessageBus& CGameBase::GetMessageBus() const
    {
        ASSERT(m_MessageBus, "MessageBus not initialized.");
        return *m_MessageBus;
    }

    CGameStateMachine& CGameBase::GetStateMachine()
    {
        return m_StateMachine;
    }
}
