#include "Game.hpp"

#include "Logger.hpp"

#include "states/GameStateInitialization.hpp"
#include "states/GameStatePlay.hpp"
#include "events/GameStateEvents.hpp"
#include "EventDispatcher.hpp"

void CGame::Configure(pulvis::game_engine::SEngineConfig& _config)
{
	_config.AppName = "pulvis";
	_config.RendererType = pulvis::rendering::ERendererType::OpenGL;
	_config.WindowWidth = 800;
	_config.WindowHeight = 600;
	_config.WindowTitle = "Pulvis";
}

void CGame::OnInitialize()
{
	PULVIS_INFO_LOG("Game initialized.");

	GetStateMachine().PushState(std::make_unique<CGameStateInitialization>(*this));

	m_GameInitializedEventHandle = GetEventDispatcher().Subscribe<SGameInitializedEvent>(
		[this](const SGameInitializedEvent&)
		{
			PULVIS_INFO_LOG("Received SGameInitializedEvent, switching to play state.");
			GetStateMachine().SwitchState(std::make_unique<CGameStatePlay>(*this));
		}
	);
}

void CGame::OnShutdown()
{
	PULVIS_INFO_LOG("Game shutdown.");
}

void CGame::Frame(float _dt)
{
}

void CGame::Render()
{
	// Global rendering that runs regardless of state.
}