#include "Game.hpp"

#include "Logger.hpp"

#include "states/GameStateInitialization.hpp"

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

	GetStateMachine().PushState(
		std::make_unique<CGameStateInitialization>(), *this);
}

void CGame::OnShutdown()
{
	PULVIS_INFO_LOG("Game shutdown.");
}

void CGame::Frame(float _dt)
{
	// Global game logic that runs regardless of state.
}

void CGame::Render()
{
	// Global rendering that runs regardless of state.
}