#include "Game.hpp"

#include "RenderSevice.hpp"
#include "LevelService.hpp"

//Game::~Game()
//{
//
//}

void Game::Initialize()
{
	pulvis::rendering::CRenderService& render_service = pulvis::rendering::CRenderService::GetInstance();
	render_service.Initialize(pulvis::rendering::ERendererType::OpenGL);

	pulvis::level::CLevelService::GetInstance().Initialize();
}

void Game::Run()
{
	while (!pulvis::rendering::CRenderService::GetInstance().GetShouldClose())
	{
		BeginFrame();
		Frame();
		EndFrame();
	}
}

void Game::Shutdown()
{
	pulvis::rendering::CRenderService::GetInstance().Shutdown();
	pulvis::level::CLevelService::GetInstance().Shutdown();
}

Game& Game::GetInstance()
{
	static Game game;
	return game;
}

void Game::BeginFrame()
{

}

void Game::Frame()
{

}

void Game::EndFrame()
{
	pulvis::rendering::CRenderService::GetInstance().Frame();
}