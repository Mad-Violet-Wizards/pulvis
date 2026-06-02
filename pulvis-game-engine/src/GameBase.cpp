#include "GameBase.hpp"

#include "DomainRoots.hpp"
#include "MountSystem.hpp"
#include "assets/AssetRegistry.hpp"
#include "MessageBus.hpp"
#include "RenderService.hpp"
#include "LevelService.hpp"
#include "Logger.hpp"
#include "loaders/ShaderLoader.hpp"
#include "FileSourceDisk.hpp"
#include "EventDispatcher.hpp"
#include "ScriptableService.hpp"
#include "loaders/ScriptLoader.hpp"
#include "EventScriptBridge.hpp"
#include "EcsService.hpp"
#include "input/InputService.hpp"
#include "animation/AnimationService.hpp"
#include "opengl/GLRenderer.hpp"

#include "components/LogicComponent.hpp"
#include "components/HierarchyComponent.hpp"
#include "components/TagComponent.hpp"
#include "components/TransformComponent.hpp"
#include "components/SpriteComponent.hpp"
#include "components/DirectionComponent.hpp"
#include "animation/components/AnimationComponent.hpp"
#include "animation/components/AnimationStateComponent.hpp"

#include "systems/LogicSystem.hpp"
#include "systems/HierarchySystem.hpp"
#include "systems/TransformSystem.hpp"
#include "systems/SpriteRenderSystem.hpp"
#include "animation/systems/AnimationSystem.hpp"


namespace pulvis::game_engine
{
	CGameBase::CGameBase()
		: m_LastFrameTime(Clock::now())
		, m_MainChannelID(pulvis::threads::INVALID_CHANNEL_ID)
		, m_RenderChannelID(pulvis::threads::INVALID_CHANNEL_ID)
		, m_AudioChannelID(pulvis::threads::INVALID_CHANNEL_ID)
		, m_IOChannelID(pulvis::threads::INVALID_CHANNEL_ID)
	{
	}

	CGameBase::~CGameBase() = default;

	void CGameBase::Run()
	{
		Configure(m_Config);
		Initialize();
		OnInitialize();
		MainLoop();
		OnShutdown();
		Shutdown();
	}

	void CGameBase::Initialize()
	{
		PULVIS_INFO_LOG("Initializing core systems...");
		m_EventDispatcher = std::make_unique<pulvis::events::CEventDispatcher>();
		InitializeFilesystem();
		InitializeMessageBus();
		InitializeServices();
		InitializeEcs();
		LoadScripts();
		PULVIS_INFO_LOG("Core systems initialized.");
	}

	void CGameBase::InitializeFilesystem()
	{
		m_DomainRoots = std::make_unique<pulvis::fs::CDomainRoots>(m_Config.AppName, m_Config.GameAssetsPath);
		m_MountSystem = std::make_unique<pulvis::fs::CMountSystem>();
		m_MountSystem->BootstrapDomains(*m_DomainRoots);
		InitializeGameFilesystems();
		m_AssetRegistry = std::make_unique<pulvis::fs::assets::CAssetRegistry>(*m_MountSystem, *m_EventDispatcher);
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
		m_RenderService = std::make_unique<pulvis::rendering::CRenderService>(*m_AssetRegistry);
		m_RenderService->Initialize(m_Config.RendererType, m_Config.WindowWidth, m_Config.WindowHeight, m_Config.WindowTitle);

		m_ScriptableService = std::make_unique<pulvis::scriptable::CScriptableService>(*m_AssetRegistry);
		m_ScriptableService->Initialize();

		m_LevelService = std::make_unique<pulvis::level::CLevelService>(*m_AssetRegistry, *m_MountSystem, *m_ScriptableService);
		m_LevelService->Initialize(*m_MessageBus, m_IOChannelID, m_MainChannelID);

		m_EcsService = std::make_unique<pulvis::ecs::CEcsService>(*m_ScriptableService, *m_AssetRegistry, *m_EventDispatcher);
		m_EcsService->Initialize();

		m_EventScriptBridge = std::make_unique<pulvis::events::CEventScriptBridge>(*m_EventDispatcher, m_ScriptableService->GetLuaState());

		m_InputService = std::make_unique<pulvis::systems::input::CInputService>(*m_EventDispatcher, *m_ScriptableService, *m_RenderService, *m_AssetRegistry);
		m_InputService->Initialize();

		m_AnimationService = std::make_unique<pulvis::systems::animation::CAnimationService>();
		m_AnimationService->Initialize(*m_AssetRegistry);

		m_AssetRegistry->RegisterLoader(pulvis::fs::EAssetType::Script,
			std::make_unique<pulvis::scriptable::CScriptLoader>());

		InitializeGameServices();

		m_ScriptableService->Initialize();
	}

	void CGameBase::InitializeEcs()
	{
		m_EcsService->RegisterComponent<pulvis::ecs::SLogicComponent>("SLogicComponent");
		m_EcsService->RegisterComponent<pulvis::ecs::SHierarchyComponent>("SHierarchyComponent");
		m_EcsService->RegisterComponent<pulvis::ecs::STagComponent>("STagComponent");
		m_EcsService->RegisterComponent<pulvis::ecs::STransformComponent>("STransformComponent");
		m_EcsService->RegisterComponent<pulvis::ecs::SSpriteComponent>("SSpriteComponent");
		m_EcsService->RegisterComponent<pulvis::ecs::SDirectionComponent>("SDirectionComponent");
		m_EcsService->RegisterComponent<pulvis::systems::animation::SAnimationComponent>("SAnimationComponent");
		m_EcsService->RegisterComponent<pulvis::systems::animation::SAnimationStateComponent>("SAnimationStateComponent");

		m_EcsService->RegisterSystem<pulvis::ecs::CLogicSystem>(*m_ScriptableService, m_EcsService->GetSignalBridge());
		m_EcsService->RegisterSystem<pulvis::ecs::CHierarchySystem>();

		m_EcsService->RegisterSystem<pulvis::ecs::CTransformSystem>();

		pulvis::rendering::CRenderService& render_service = GetRenderService();
		pulvis::rendering::gl::CGLRenderer* gl_renderer = static_cast<pulvis::rendering::gl::CGLRenderer*>(render_service.GetRenderer());
		m_EcsService->RegisterSystem<pulvis::ecs::CSpriteRenderSystem>(gl_renderer->GetRenderQueue(), gl_renderer->GetLayerCache());

		m_EcsService->RegisterSystem<pulvis::systems::animation::CAnimationSystem>(m_AnimationService->GetEventManager());
	}

	void CGameBase::LoadScripts()
	{
		m_EcsService->LoadAndExecuteScripts();
		m_InputService->LoadAndExecuteScripts();
	}

	void CGameBase::Shutdown()
	{
		PULVIS_INFO_LOG("Shutting down core systems...");
		m_InputService.reset();
		m_EventScriptBridge.reset();
		m_EcsService.reset();
		m_LevelService.reset();
		m_RenderService.reset();
		m_ScriptableService.reset();
		m_AssetRegistry.reset();
		m_MessageBus.reset();
		m_EventDispatcher.reset();
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
			if (m_LevelService->HasActiveLevel()) { m_LevelService->ApplyPendingChunks(); }
			m_InputService->Frame(dt);
			m_EventDispatcher->Frame(dt);
			m_EcsService->Frame(dt);
			m_StateMachine.Frame(dt);
			Frame(dt);

			m_RenderService->BeginFrame();
			m_StateMachine.Render();
			m_RenderService->Frame();
			Render();
			m_RenderService->EndFrame();
		}
	}

	pulvis::fs::CDomainRoots& CGameBase::GetDomainRoots() const { ASSERT(m_DomainRoots, "DomainRoots not initialized.");           return *m_DomainRoots; }
	pulvis::fs::CMountSystem& CGameBase::GetMountSystem() const { ASSERT(m_MountSystem, "MountSystem not initialized.");           return *m_MountSystem; }
	pulvis::fs::assets::CAssetRegistry& CGameBase::GetAssetRegistry() const { ASSERT(m_AssetRegistry, "AssetRegistry not initialized.");       return *m_AssetRegistry; }
	pulvis::rendering::CRenderService& CGameBase::GetRenderService() const { ASSERT(m_RenderService, "RenderService not initialized.");       return *m_RenderService; }
	pulvis::level::CLevelService& CGameBase::GetLevelService() const { ASSERT(m_LevelService, "LevelService not initialized.");         return *m_LevelService; }
	pulvis::scriptable::CScriptableService& CGameBase::GetScriptableService() const { ASSERT(m_ScriptableService, "ScriptableService not initialized."); return *m_ScriptableService; }
	pulvis::ecs::CEcsService& CGameBase::GetEcsService() const { ASSERT(m_EcsService, "EcsService not initialized.");             return *m_EcsService; }
	pulvis::threads::CMessageBus& CGameBase::GetMessageBus() const { ASSERT(m_MessageBus, "MessageBus not initialized.");             return *m_MessageBus; }
	pulvis::events::CEventDispatcher& CGameBase::GetEventDispatcher() const { ASSERT(m_EventDispatcher, "EventDispatcher not initialized.");   return *m_EventDispatcher; }
	pulvis::events::CEventScriptBridge& CGameBase::GetEventScriptBridge() const { ASSERT(m_EventScriptBridge, "EventScriptBridge not initialized."); return *m_EventScriptBridge; }
	pulvis::systems::input::CInputService& CGameBase::GetInputService() const { ASSERT(m_InputService, "InputService not initialized."); return *m_InputService; }
	pulvis::systems::animation::CAnimationService& CGameBase::GetAnimationService() const { ASSERT(m_AnimationService, "AnimationService not initialized."); return *m_AnimationService; }

	CGameStateMachine& CGameBase::GetStateMachine() { return m_StateMachine; }

	uint32_t CGameBase::GetMainChannelID() const { return m_MainChannelID; }
	uint32_t CGameBase::GetRenderChannelID() const { return m_RenderChannelID; }
	uint32_t CGameBase::GetAudioChannelID() const { return m_AudioChannelID; }
	uint32_t CGameBase::GetIOChannelID() const { return m_IOChannelID; }
}