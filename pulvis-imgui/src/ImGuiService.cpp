#include "ImGuiService.hpp"
#include "ImguiBackend.hpp"
#include "widgets/PanelManager.hpp"
#include "Assert.hpp"

#include <imgui/imgui.h>
#include <map>

namespace pulvis::imgui
{
	CImGuiService::CImGuiService()
		: m_Backend(nullptr)
		, m_PanelManager(std::make_unique<widgets::CPanelManager>())
		, m_Initialized(false)
	{
	}

	CImGuiService::~CImGuiService()
	{
		if (m_Initialized)
		{
			Shutdown();
		}
	}

	bool CImGuiService::Initialize(std::unique_ptr<IImGuiBackend> _backend, const SImGuiServiceConfig& _config)
	{
		if (m_Initialized)
		{
			return true;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		if (_config.EnableKeyboardNavigation)
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		if (_config.EnableGamepadNavigation)
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		if (!_config.IniFilename.empty())
		{
			m_IniFilename.assign(_config.IniFilename);
			io.IniFilename = m_IniFilename.c_str();
		}

		ApplyTheme();

		if (!_config.FontPath.empty())
		{
			LoadFont(_config.FontPath, _config.FontSize);
		}

		if (!_backend->Initialize())
		{
			ASSERT(false, "Failed to initialize ImGui context!");
			ImGui::DestroyContext();
			return false;
		}

		m_Backend = std::move(_backend);
		m_Initialized = true;
		return true;
	}

	void CImGuiService::Shutdown()
	{
		if (!m_Initialized)
		{
			return;
		}

		m_PanelManager->Clear();
		m_PanelManager.reset();

		m_Backend->Shutdown();
		m_Backend.reset();

		ImGui::DestroyContext();
		m_Initialized = false;
	}

	void CImGuiService::BeginFrame()
	{
		if (!m_Initialized)
		{
			return;
		}

		m_Backend->BeginFrame();
	}

	void CImGuiService::Frame(float _dt)
	{
		if (!m_Initialized)
		{
			return;
		}

		if (m_State == EImGuiState::Hidden)
		{
			return;
		}

		m_PanelManager->Frame(_dt);
	}

	void CImGuiService::Render()
	{
		if (!m_Initialized)
		{
			return;
		}

		if (m_State != EImGuiState::Hidden)
		{
			RenderMainMenuBar();
			m_PanelManager->Render();
		}

		m_Backend->Render();
	}

	void CImGuiService::ToggleState()
	{
		switch (m_State)
		{
			case EImGuiState::Active: SetState(EImGuiState::Passive); break;
			case EImGuiState::Passive: SetState(EImGuiState::Hidden); break;
			case EImGuiState::Hidden: SetState(EImGuiState::Active); break;
		}
	}

	void CImGuiService::SetState(EImGuiState _state)
	{
		m_State = _state;
		ApplyState();
	}

	void CImGuiService::ApplyTheme()
	{
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 4.0f;
		style.FrameRounding = 3.0f;
		style.GrabRounding = 3.0f;
		style.TabRounding = 3.0f;
		style.ScrollbarRounding = 3.0f;
		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.FramePadding = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(6.0f, 4.0f);
	}

	void CImGuiService::LoadFont(const std::string_view _font_path, float _font_size)
	{
		ImGuiIO& io = ImGui::GetIO();
		const ImFont* font = io.Fonts->AddFontFromFileTTF(_font_path.data(), _font_size);

		if (!font)
		{
			const std::string error_message = "Failed to load font from path: " + std::string(_font_path);
			ASSERT(false, error_message.c_str());
			io.Fonts->AddFontDefault();
			return;
		}
	}

	void CImGuiService::ApplyState()
	{
		if (!m_Initialized)
		{
			return;
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		switch (m_State)
		{
		case EImGuiState::Active:
			io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			io.ConfigFlags &= ~ImGuiConfigFlags_NoKeyboard;
			style.Alpha = 1.0f;
			break;
		case EImGuiState::Passive:
			io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
			io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
			style.Alpha = 0.35f;
			break;
		case EImGuiState::Hidden:
			io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
			io.ConfigFlags |= ImGuiConfigFlags_NoKeyboard;
			style.Alpha = 1.0f;
			break;
		}
	}

	void CImGuiService::RenderMainMenuBar()
	{
		if (!ImGui::BeginMainMenuBar())
		{
			return;
		}

		std::map<std::string_view, std::vector<widgets::IPanel*>> grouped;

		m_PanelManager->ForEach(
			[&grouped](widgets::panel_id_t /*_id*/, widgets::IPanel& _panel)
			{
				grouped[_panel.GetMenuName()].push_back(&_panel);
			});

		for (auto& [menu_name, panels] : grouped)
		{
			const std::string menu_label(menu_name);
			if (ImGui::BeginMenu(menu_label.c_str()))
			{
				for (widgets::IPanel* panel : panels)
				{
					bool visible = panel->IsVisible();
					const std::string item_label(panel->GetName());
					if (ImGui::MenuItem(item_label.c_str(), nullptr, &visible))
					{
						panel->SetVisible(visible);
					}
				}
				ImGui::EndMenu();
			}
		}

		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::TextDisabled("Press '~' to toggle ImGui state");
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}