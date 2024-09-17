#include "engine/engine_pch.hpp"

#include "Application.hpp"
#include "Setup.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/events/EventController.hpp"

namespace engine
{
	namespace core
	{
		Application::Application(const ApplicationSetup& _app_setup)
			: m_EngineFilesystem("engine", engine::fs::GetEnginePath() + "/pulvis/")
			, m_bCoreSystemsInitialized(false)
			, m_FrameTime(0.f)
		{
			InitializeCoreSystems(_app_setup);
		}

		Application::~Application()
		{
			delete m_Window;
		}


		void Application::InitializeCoreSystems(const ApplicationSetup& _app_setup)
		{
			engine::events::CEventController::Init();
			CAssertManager::Init();
			CLogger::Init();

			m_EngineFilesystem.Mount();

			InitializeWindow(_app_setup);

			m_bCoreSystemsInitialized = true;
		}

		void Application::InitializeWindow(const ApplicationSetup& _app_setup)
		{
			m_Window = new rendering::CWindow(_app_setup.m_WindowWidth, _app_setup.m_WindowHeight, _app_setup.m_WindowName.c_str());
		}

		bool Application::IsCloseRequested() const
		{
			return m_Window->ShouldClose();
		}

		void Application::Run()
		{
			while(!IsCloseRequested())
			{
				PreFrame();
				Frame();
				PostFrame();
				glfwPollEvents();
			}
		}

		void Application::PreFrame()
		{
			m_FrameTimer.Start();
			engine::events::CEventController::GetInstance().PreFrame();
		}

		void Application::Frame()
		{
			engine::events::CEventController::GetInstance().ProcessEvents();
		}

		void Application::PostFrame()
		{
			engine::events::CEventController::GetInstance().PostFrame();
			UpdateFrameTime();
		}
		
		void Application::UpdateFrameTime()
		{
			m_FrameTimer.End();
			m_FrameTime = m_FrameTimer.GetElapsedTime(engine::time::ETimeUnit::Milliseconds);
			m_FrameTimer.Reset();
		}

		const engine::fs::Filesystem& Application::GetEngineFs() const
		{
			return m_EngineFilesystem;
		}
	}
}