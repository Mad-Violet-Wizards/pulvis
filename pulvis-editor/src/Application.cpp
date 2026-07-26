#include "Application.hpp"

#include "EngineConfig.hpp"
#include "DomainRoots.hpp"
#include "RenderService.hpp"
#include "opengl/GLRenderer.hpp"

#include "ImGuiBackend_GLFW_GL3.hpp"
#include "widgets/PanelManager.hpp"
#include "widgets/LogPanel.hpp"
#include "widgets/DockSpacePanel.hpp"

#include "Logger.hpp"

#include <imgui/imgui.h>

namespace pulvis::editor
{
	constexpr const char* GAME_MODULE_RELATIVE_PATH =
#if defined(WINDOWS_OS) && defined(DEBUG)
		"../build/Debug/gfz-game.dll";
#elif defined(WINDOWS_OS) && defined(RELEASE)
		"../build/Release/gfz-game.dll";
#elif defined (MAC_OS) && defined(DEBUG)
		"../build/Debug/libgfz-game.dylib";
#elif defined (MAC_OS) && defined(RELEASE)
		"../build/Release/libgfz-game.dylib";
#endif

	void CApplication::Configure(pulvis::game_engine::SEngineConfig& _engine_config)
	{
		_engine_config.AppName = "pulvis-editor";
		_engine_config.WindowTitle = "Pulvis Editor";
		_engine_config.WindowWidth = 1280;
		_engine_config.WindowHeight = 720;
	}

	void CApplication::ConfigureDomainsRoots()
	{
		constexpr const char* PROJECT_NAME = "gfz";
		const std::filesystem::path workspace_root = std::filesystem::current_path().parent_path();

		const std::filesystem::path project_assets_root = workspace_root / PROJECT_NAME / std::format("{}-assets", PROJECT_NAME);

		GetDomainRoots().OverrideRoot(pulvis::fs::EDomain::Game, project_assets_root);
	}

	void CApplication::InitializeGameFilesystems()
	{
	}
	
	void CApplication::InitializeGameServices()
	{
	}

	void CApplication::OnInitialize()
	{
		InitializeImGui();
		RegisterPanels();

		const EGameModuleState status = m_GameModule.Load(GetDomainRoots(), pulvis::fs::EDomain::Game, GAME_MODULE_RELATIVE_PATH);

		if (status != EGameModuleState::Loaded)
		{
			PULVIS_ERROR_LOG("Failed to load game module: {}", GAME_MODULE_RELATIVE_PATH);
		}
	}

	void CApplication::OnShutdown()
	{
		m_GameModule.Unload();

		if (m_ImGuiService && m_ImGuiService->IsInitialized())
		{
			m_ImGuiService->Shutdown();
			m_ImGuiService.reset();
		}
	}

	void CApplication::Frame(float _dt)
	{
		if (!m_ImGuiService || !m_ImGuiService->IsInitialized())
		{
			return;
		}

		m_ImGuiService->BeginFrame();
		RenderDockSpace();
		HandleInputs();
		m_ImGuiService->Frame(_dt);
	}

	void CApplication::Render()
	{
		if (!m_ImGuiService || !m_ImGuiService->IsInitialized())
		{
			return;
		}

		m_ImGuiService->Render();
	}

	void CApplication::InitializeImGui()
	{
		pulvis::rendering::gl::CGLRenderer* renderer = static_cast<pulvis::rendering::gl::CGLRenderer*>(GetRenderService().GetRenderer());
		GLFWwindow* native_window = renderer->GetWindow().GetNativeWindow();

		m_ImGuiService = std::make_unique<pulvis::imgui::CImGuiService>();

		pulvis::imgui::SImGuiServiceConfig imgui_config;
		imgui_config.IniFilename = "editor.ini";
		imgui_config.EnableDocking = true;

		std::unique_ptr<pulvis::imgui::CImGuiBackend_GLFW_GL3> imgui_backend = std::make_unique<pulvis::imgui::CImGuiBackend_GLFW_GL3>(native_window, "#version 330");

		if (!m_ImGuiService->Initialize(std::move(imgui_backend), imgui_config))
		{
			PULVIS_ERROR_LOG("Failed to initialize ImGui service.");
			return;
		}

		m_ImGuiService->SetState(pulvis::imgui::EImGuiState::Active);
	}

	void CApplication::RegisterPanels()
	{
		pulvis::imgui::widgets::CPanelManager& panel_manager = m_ImGuiService->GetPanelManager();
		panel_manager.Register(std::make_unique<pulvis::imgui::widgets::CLogPanel>());
	}

	void CApplication::RenderDockSpace()
	{
	}

	void CApplication::HandleInputs()
	{
	}
}
