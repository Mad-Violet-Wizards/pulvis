#include "GameStateInitialization.hpp"

#include "GameBase.hpp"
#include "assets/AssetRegistry.hpp"
#include "RenderSevice.hpp"
#include "loaders/ShaderLoader.hpp"
#include "Logger.hpp"
#include "GameStatePlay.hpp"
#include "events/GameStateEvents.hpp"
#include "EventDispatcher.hpp"

CGameStateInitialization::CGameStateInitialization(pulvis::game_engine::CGameBase& _game)
	: IGameState(_game)
{
}

void CGameStateInitialization::OnEnter()
{
	PULVIS_INFO_LOG("[Init] Loading core assets...");

	pulvis::fs::assets::CAssetRegistry& registry = m_Game.GetAssetRegistry();
	registry.RegisterLoader(pulvis::fs::EAssetType::Shader,
		std::make_unique<pulvis::rendering::CShaderLoader>());

	// Pre-load all engine shaders through the asset registry.
	// This populates RawData so renderers can compile on first use.
	registry.LoadDirectory(pulvis::fs::EDomain::Engine, "engine/shaders", true);

	m_LoadingComplete = true;

	PULVIS_INFO_LOG("[Init] Core assets loaded.");
}

void CGameStateInitialization::OnExit()
{
	PULVIS_INFO_LOG("[GameStateInit] Initialization state exiting.");
	m_Game.GetEventDispatcher().EnqueueFrameDelay<SGameInitializedEvent>(1);
}

void CGameStateInitialization::Frame(float _dt)
{
}

void CGameStateInitialization::Render()
{
	// Render splash screen / loading bar.
}

bool CGameStateInitialization::WantsExit() const
{
	return m_LoadingComplete;
}