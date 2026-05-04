#include "StatsPanel.hpp"

#include <imgui/imgui.h>

namespace pulvis::imgui::widgets
{
	void CStatsPanel::Frame(float _dt)
	{
		m_Accumulator += _dt;
		m_FrameCounter++;
		if (m_Accumulator >= 1.f)
		{
			m_FPS = m_FrameCounter / m_Accumulator;
			m_FrameTimeMs = (m_FPS > 0.0f) ? (1000.0f / m_FPS) : 0.0f;
			m_Accumulator = 0.f;
			m_FrameCounter = 0;
		}
	}

	void CStatsPanel::Render()
	{
		if (!ImGui::Begin("Stats", &m_Visible))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("FPS:        %6.1f", m_FPS);
		ImGui::Text("Frame time: %6.2f ms", m_FrameTimeMs);

		ImGui::End();
	}
}