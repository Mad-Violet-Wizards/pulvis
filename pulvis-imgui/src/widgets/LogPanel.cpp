#include "LogPanel.hpp"
#include "FastFunction.hpp"

#include <imgui/imgui.h>

namespace pulvis::imgui::widgets
{
  namespace
  {
    ImVec4 ColorForLevel(pulvis::core::ELogLevel _level)
    {
      using pulvis::core::ELogLevel;
      switch (_level)
      {
      case ELogLevel::Fatal:   return ImVec4(1.00f, 0.20f, 0.20f, 1.0f);
      case ELogLevel::Error:   return ImVec4(1.00f, 0.35f, 0.35f, 1.0f);
      case ELogLevel::Warning: return ImVec4(1.00f, 0.80f, 0.20f, 1.0f);
      case ELogLevel::Info:    return ImVec4(0.55f, 0.75f, 1.00f, 1.0f);
      case ELogLevel::Debug:   return ImVec4(0.70f, 0.70f, 0.70f, 1.0f);
      }
      return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
  }

  CLogPanel::CLogPanel(std::size_t _max_entries)
    : m_MaxEntries(_max_entries == 0 ? 1 : _max_entries)
  {
  }

  CLogPanel::~CLogPanel()
  {
    OnDetach();
  }

  void CLogPanel::OnAttach()
  {
    if (m_SinkId != pulvis::core::INVALID_LOG_SINK_ID)
    {
      return;
    }

    auto sink_fn = [this](const pulvis::core::SLogInfo& _info)
    {
      std::lock_guard<std::mutex> lock(m_EntriesMutex);
      if (m_Entries.size() >= m_MaxEntries)
      {
        m_Entries.pop_front();
      }
      m_Entries.push_back(_info);
		};

    pulvis::tl::FastFunction sink = pulvis::tl::FastFunction::Make<void, const pulvis::core::SLogInfo&>(sink_fn);
    m_SinkId = pulvis::core::CLogService::Get().RegisterSink(std::move(sink));
  }

  void CLogPanel::OnDetach()
  {
    if (m_SinkId != pulvis::core::INVALID_LOG_SINK_ID)
    {
      pulvis::core::CLogService::Get().UnregisterSink(m_SinkId);
      m_SinkId = pulvis::core::INVALID_LOG_SINK_ID;
    }
  }

  void CLogPanel::Clear()
  {
    std::lock_guard<std::mutex> lock(m_EntriesMutex);
    m_Entries.clear();
  }

  void CLogPanel::Render()
  {
    if (!ImGui::Begin("Log", &m_Visible))
    {
      ImGui::End();
      return;
    }

    if (ImGui::Button("Clear")) { Clear(); }
    ImGui::SameLine(); ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
    ImGui::SameLine(); ImGui::Checkbox("F", &m_ShowFatal);
    ImGui::SameLine(); ImGui::Checkbox("E", &m_ShowError);
    ImGui::SameLine(); ImGui::Checkbox("W", &m_ShowWarning);
    ImGui::SameLine(); ImGui::Checkbox("I", &m_ShowInfo);
    ImGui::SameLine(); ImGui::Checkbox("D", &m_ShowDebug);

    ImGui::Separator();

    if (ImGui::BeginChild("##LogScroll", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
      std::lock_guard<std::mutex> lock(m_EntriesMutex);
      for (const auto& entry : m_Entries)
      {
        using pulvis::core::ELogLevel;
        const bool show =
          (entry.m_LogLevel == ELogLevel::Fatal && m_ShowFatal) ||
          (entry.m_LogLevel == ELogLevel::Error && m_ShowError) ||
          (entry.m_LogLevel == ELogLevel::Warning && m_ShowWarning) ||
          (entry.m_LogLevel == ELogLevel::Info && m_ShowInfo) ||
          (entry.m_LogLevel == ELogLevel::Debug && m_ShowDebug);

        if (!show) { continue; }

        ImGui::PushStyleColor(ImGuiCol_Text, ColorForLevel(entry.m_LogLevel));
        ImGui::TextUnformatted(pulvis::core::ToString(entry.m_LogLevel).c_str());
        ImGui::SameLine();
        ImGui::TextUnformatted(entry.m_Message.c_str());
        ImGui::PopStyleColor();
      }

      if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
      {
        ImGui::SetScrollHereY(1.0f);
      }
    }
    ImGui::EndChild();

    ImGui::End();
  }
} // namespace pulvis::imgui