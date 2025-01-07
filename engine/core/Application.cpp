#include "engine/engine_pch.hpp"

#include "Application.hpp"
#include "engine/filesystem/Utils.hpp"
#include "engine/events/EventController.hpp"
#include "engine/rendering/RenderingService.hpp"
#include <GLFW/glfw3.h>

namespace engine
{
	namespace core
	{
//////////////////////////////////////////////////////////////////////////
	extern PULVIS_API SApplicationContext s_AppContext = {};
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

			InitializeServices();
		}

		Application::~Application()
		{
		}


		void Application::InitializeServices()
		{
			m_EngineFilesystem.Mount();

			engine::rendering::RenderingService::GetInstance().Initialize(engine::rendering::ERendererType::Vulkan);
		}

		bool Application::IsCloseRequested() const
		{
			return engine::rendering::RenderingService::GetInstance().ShouldClose();
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