#include "engine/engine_pch.hpp"

#include "Application.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/events/EventController.hpp"

#include <GLFW/glfw3.h>

namespace engine
{
	namespace core
	{
//////////////////////////////////////////////////////////////////////////
		SFrameContext::SFrameContext()
			: m_FrameNumber(0)
			, m_DeltaTime(0.f)
		{
		}

		extern PULVIS_API SFrameContext s_FrameContext = {};

//////////////////////////////////////////////////////////////////////////
		Application::Application(const SApplicationSetup& _app_setup)
			: m_EngineFilesystem("engine", engine::fs::GetEnginePath() + "/pulvis/")
			, m_bCoreSystemsInitialized(false)
			, m_FrameTime(0.f)
		{
			ASSERT(_app_setup.m_ClientApp != EClientApp::Unknown, "Client application not set!");

			s_AppContext.m_AppSetup = _app_setup;

			InitializeCoreSystems();
		}

		Application::~Application()
		{
			if (s_AppContext.m_AppSetup.m_ClientApp != EClientApp::Playground)
				delete m_Window;
		}


		void Application::InitializeCoreSystems()
		{
			m_EngineFilesystem.Mount();

			if (s_AppContext.m_AppSetup.m_ClientApp != EClientApp::Playground)
				InitializeWindow();
		}

		void Application::InitializeWindow()
		{
			const std::string window_name = s_AppContext.m_AppSetup.m_ApplicationName;
			m_Window = new rendering::CWindow(s_AppContext.m_AppSetup.m_WindowWidth, s_AppContext.m_AppSetup.m_WindowHeight, window_name.c_str());
		}

		bool Application::IsCloseRequested() const
		{
			return m_Window->ShouldClose();
		}

		void Application::Run()
		{
			while(!IsCloseRequested())
			{
				FrameCycle();
				glfwPollEvents();
			}
		}

		void Application::FrameCycle()
		{
			PreFrame();
			Frame();
			PostFrame();
		}

		void Application::PreFrame()
		{
			s_FrameContext.m_FrameNumber++;
			s_FrameContext.m_DeltaTime = m_FrameTime;

			m_FrameTimer.Start();
		}

		void Application::Frame()
		{
			engine::events::CEventController::GetInstance().Frame();
		}

		void Application::PostFrame()
		{
			UpdateFrameTime();
		}
		
		void Application::UpdateFrameTime()
		{
			m_FrameTimer.End();
			m_FrameTime = static_cast<float>(m_FrameTimer.GetElapsedTime(engine::time::ETimeUnit::Milliseconds));
			m_FrameTimer.Reset();
		}

		const engine::fs::Filesystem& Application::GetEngineFs() const
		{
			return m_EngineFilesystem;
		}
	}
}