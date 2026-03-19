#include "Game.hpp"

#include "RenderSevice.hpp"
#include "LevelService.hpp"
#include "MessageBus.hpp"
#include "Logger.hpp"
#include "StackUsage.hpp"

#include <cstdint>
#include <fmt/core.h>

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


//Game::~Game()
//{
//
//}

void Game::Initialize()
{
	pulvis::rendering::CRenderService& render_service = pulvis::rendering::CRenderService::GetInstance();
	render_service.Initialize(pulvis::rendering::ERendererType::OpenGL);

	pulvis::level::CLevelService::GetInstance().Initialize();

	InitializeMessageBuses();
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

#if defined(WINDOWS_OS)
	auto info = pulvis::threads::StackUsage::QueryStackFromOS();
	PULVIS_INFO_LOG("Stack: {:.1f} KB / {:.1f} KB ({:.1f}% used)\r",
		info.Used / 1024.0,
		info.Reserved / 1024.0,
		100.0 * info.Used / info.Reserved);
#endif
}

void Game::InitializeMessageBuses()
{
	pulvis::threads::CMessageBus bus;
	bus.RegisterChannel(EChannel::Main, 4096);
	bus.RegisterChannel(EChannel::Render, 4096);
	bus.RegisterChannel(EChannel::Audio, 4096);

	int resize_count = 0;
	int* rc = &resize_count;

	bus.RegisterHandler<SWindowResized>(EChannel::Main,
		[rc](const SWindowResized& _msg) {
			*rc += 1;
			PULVIS_INFO_LOG("[Main] WindowResized: {}x{}", _msg.Width, _msg.Height);
		}
	);

	bus.RegisterHandler<SAudioPlayCmd>(EChannel::Audio,
		+[](const SAudioPlayCmd& _msg) {
			PULVIS_INFO_LOG("[Audio] Play sound {} at volume {:.2f}", _msg.SoundId, _msg.Volume);
		}
	);

	bus.RegisterHandler<SLoadLevelCmd>(EChannel::Main,
		+[](const SLoadLevelCmd& _msg) {
			PULVIS_INFO_LOG("[Main] Load level {}", _msg.LevelId);
		}
	);


	bus.Send<SWindowResized>(EChannel::Main, SWindowResized{ 1920, 1080 });
	bus.Send<SWindowResized>(EChannel::Main, SWindowResized{ 2560, 1440 });
	bus.Send<SAudioPlayCmd>(EChannel::Audio, SAudioPlayCmd{ 42, 0.8f });
	bus.Send<SLoadLevelCmd>(EChannel::Main, SLoadLevelCmd{ 1 });

	bus.Send<SAudioPlayCmd>(EChannel::Render, SAudioPlayCmd{ 99, 1.0f });

	PULVIS_INFO_LOG("Draining Main channel");
	bus.Drain(EChannel::Main);

	PULVIS_INFO_LOG("Draining Audio channel");
	bus.Drain(EChannel::Audio);

	PULVIS_INFO_LOG("Draining Render channel (no handlers)");
	bus.Drain(EChannel::Render);

	PULVIS_INFO_LOG("MessageBus smoke test: resize_count = {} (expected 2)", resize_count);
}
