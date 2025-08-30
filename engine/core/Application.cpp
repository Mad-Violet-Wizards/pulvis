#include "engine/engine_pch.hpp"

#include "Application.hpp"
#include "engine/engine_rtti_autogen.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/events/EventController.hpp"
#include "engine/rendering/RenderingService.hpp"
#include "engine/game/GameService.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine
{
	namespace core
	{

		CApplicationContext Application::m_AppContext = {};
		
//////////////////////////////////////////////////////////////////////////
		Application::Application(const SApplicationSetup& _app_setup)
			: m_FrameTime(0.f)
		{
			ASSERT(_app_setup.m_ClientApp != EClientApp::Unknown, "Client application not set!");

			RegisterRTTI_engine();

			const std::string fs_absolute_path = engine::fs::GetEnginePath() + "/pulvis/";
			m_AppContext.m_Filesystem = new engine::fs::Filesystem("Engine", fs_absolute_path);
			m_AppContext.m_Setup = _app_setup;
			m_AppContext.m_StateMachine.SetContext(&m_AppContext);
			m_AppContext.m_StateMachine.SetState(EApplicationState::Initialize);
		}

		Application::~Application()
		{
		}

		bool Application::IsCloseRequested() const
		{
			return engine::rendering::RenderingService::GetInstance().ShouldClose();
		}

		void Application::Run()
		{
			while(!IsCloseRequested())
			{
				PreFrame();
				Frame();
				PostFrame();
			}
		}

		void Application::SimulateFrame()
		{
			PreFrame();
			Frame();
			PostFrame();
		}

		void Application::PreFrame()
		{
			m_AppContext.m_FrameNumber++;
			m_AppContext.m_DeltaTime = m_FrameTime;

			m_FrameTimer.Start();

			engine::rendering::RenderingService::GetInstance().BeginFrame();
		}

		void Application::Frame()
		{
			m_AppContext.m_StateMachine.Frame();
			engine::game::CGameService::GetInstance().Frame();
			engine::events::CEventController::GetInstance().Frame();
			engine::rendering::RenderingService::GetInstance().Frame();
			m_AppContext.m_ThreadTaskScheduler.Process();
		}

		void Application::PostFrame()
		{
			engine::rendering::RenderingService::GetInstance().EndFrame();
			UpdateFrameTime();
		}
		
		void Application::UpdateFrameTime()
		{
			m_FrameTimer.End();
			m_FrameTime = static_cast<float>(m_FrameTimer.GetElapsedTime(engine::time::ETimeUnit::Milliseconds));
			m_FrameTimer.Reset();
		}
	}
}