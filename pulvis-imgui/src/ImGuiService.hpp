#pragma once

#include <memory>
#include <string_view>
#include <string>

namespace pulvis::imgui
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	class IImGuiBackend;

	namespace widgets
	{
		class CPanelManager;
	} // namespace pulvis::imgui::widgets

///////////////////////////////////////////////////////////////////////////////////////////////////
	struct SImGuiServiceConfig
	{
		bool EnableKeyboardNavigation = true;
		bool EnableGamepadNavigation = true;
		std::string_view IniFilename = {};
		std::string_view FontPath = {};
		float FontSize = 16.f; // [px]
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	enum class EImGuiState : uint8_t
	{
		Active, // Visible + focused (captures input).
		Passive, // Visible + transparent, (does not capture input).
		Hidden // Not rendered.
	};

///////////////////////////////////////////////////////////////////////////////////////////////////
	class CImGuiService final
	{
		public:
			CImGuiService();
			~CImGuiService();
			CImGuiService(const CImGuiService&) = delete;
			CImGuiService& operator=(const CImGuiService&) = delete;

			CImGuiService(CImGuiService&&) = delete;
			CImGuiService& operator=(CImGuiService&&) = delete;

			[[nodiscard]] bool Initialize(std::unique_ptr<IImGuiBackend> _backend, const SImGuiServiceConfig& _config = {});
			void Shutdown();
			void BeginFrame();
			void Frame(float _dt);
			void Render();

			void ToggleState();
			void SetState(EImGuiState _state);
			[[nodiscard]] EImGuiState GetState() const { return m_State; }
			[[nodiscard]] bool WantsCaptureInput() const { return m_State == EImGuiState::Active; }

			[[nodiscard]] bool IsInitialized() const { return m_Initialized; }
			[[nodiscard]] widgets::CPanelManager& GetPanelManager() { return *m_PanelManager; }
			[[nodiscard]] const widgets::CPanelManager& GetPanelManager() const { return *m_PanelManager; }

		private:

			void ApplyTheme();
			void LoadFont(const std::string_view _font_path, float _font_size);
			void ApplyState();
			void RenderMainMenuBar();

		private:
			std::unique_ptr<IImGuiBackend> m_Backend;
			std::unique_ptr<widgets::CPanelManager> m_PanelManager;
			std::string m_IniFilename;
			EImGuiState m_State = EImGuiState::Hidden;
			bool m_Initialized;
	};
}// namespace pulvis::imgui