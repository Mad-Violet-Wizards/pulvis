#include "GameStateInitialization.hpp"

#include "GameBase.hpp"
#include "assets/AssetRegistry.hpp"
#include "RenderSevice.hpp"
#include "loaders/ShaderLoader.hpp"
#include "Logger.hpp"

void CGameStateInitialization::OnEnter(pulvis::game_engine::CGameBase& _game)
{
	PULVIS_INFO_LOG("[Init] Loading core assets...");

	pulvis::fs::assets::CAssetRegistry& registry = _game.GetAssetRegistry();

	registry.RegisterLoader(pulvis::fs::EAssetType::Shader,
		std::make_unique<pulvis::rendering::CShaderLoader>());

	// Pre-load all engine shaders through the asset registry.
	// This populates RawData so renderers can compile on first use.
	registry.LoadDirectory(pulvis::fs::EDomain::Engine, "engine/shaders", true);

	m_LoadingComplete = true;

	PULVIS_INFO_LOG("[Init] Core assets loaded.");
}

void CGameStateInitialization::OnExit(pulvis::game_engine::CGameBase& _game)
{
	PULVIS_INFO_LOG("[GameStateInit] Initialization state exiting.");
}

void CGameStateInitialization::Frame(pulvis::game_engine::CGameBase& _game, float _dt)
{
	if (m_LoadingComplete)
	{
		// Transition to main menu or gameplay.
		// _game.GetStateMachine().SwitchState(
		//     std::make_unique<CGameStateMainMenu>(), _game);
	}
}

void CGameStateInitialization::Render(pulvis::game_engine::CGameBase& _game)
{
	// Render splash screen / loading bar.
}

bool CGameStateInitialization::WantsExit() const
{
	return false;
}