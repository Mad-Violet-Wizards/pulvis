#pragma once

#include "GameBase.hpp"
#include "GameModule.hpp"
#include "ImGuiService.hpp"

#include <memory>

namespace pulvis::editor
{
	class CApplication : public pulvis::game_engine::CGameBase
	{
		public:

			CApplication() = default;
			~CApplication() = default;

			CApplication(const CApplication&) = delete;
			CApplication& operator=(const CApplication&) = delete;

			void Configure(pulvis::game_engine::SEngineConfig& _engine_config) override;
			void ConfigureDomainsRoots() override;
			void InitializeGameFilesystems() override;
			void InitializeGameServices() override;

			void OnInitialize() override;
			void OnShutdown() override;
			void Frame(float _dt) override;
			void Render() override;

		private:

			void InitializeImGui();
			void RegisterPanels();
			void RenderDockSpace();
			void HandleInputs();

		private:

			std::unique_ptr<pulvis::imgui::CImGuiService> m_ImGuiService;
			CGameModule m_GameModule;
	};
}