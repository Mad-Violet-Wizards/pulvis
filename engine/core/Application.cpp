#include "engine/engine_pch.hpp"

#include "Application.hpp"
#include "Setup.hpp"

namespace engine
{
	namespace core
	{
		Application::Application(const ApplicationSetup& _app_setup)
			: m_Window(_app_setup.m_WindowWidth, _app_setup.m_WindowHeight, _app_setup.m_WindowName.c_str())
			, m_bCoreSystemsInitialized(false)
			, m_FrameTime(0.f)
		{
			// Initialize assertions
			// Initialize filesystems
			// Initialize logging
			InitializeCoreSystems();
		}

		void Application::InitializCoreeSystems()
		{
			Singleton<CAssertManager>::Init();
			Singleton<Logger>::Init();
			m_bCoreSystemsInitialized = true;
		}

		bool Application::IsCloseRequested() const
		{
			return m_Window.ShouldClose();
		}

		void Application::Run()
		{
			while(!IsCloseRequested() && m_bCoreSystemsInitialized)
			{
				StartFrame();
				FinishFrame();
				glfwPollEvents();
			}
		}

		void Application::StartFrame()
		{
			m_FrameTimer.Start();
			Frame_InputUpdate();
			Frame_UpdateSceneStateMachine();
		}

		void Application::Frame_InputUpdate()
		{

		}

		void Application::Frame_UpdateSceneStateMachine()
		{

		}

		void Application::FinishFrame()
		{
			UpdateFrameTime();
		}
		
		void Application::UpdateFrameTime()
		{
			m_FrameTimer.End();
			m_FrameTime = m_FrameTimer.GetElapsedTime(engine::time::ETimeUnit::Milliseconds);
			m_FrameTimer.Reset();
		}
	}
}