#pragma once

#include "Panel.hpp"
#include "Logger.hpp"

#include <cstddef>
#include <string>
#include <deque>
#include <mutex>

namespace pulvis::imgui::widgets
{
	class CLogPanel final : public IPanel
	{
	public:

		explicit CLogPanel(std::size_t _max_entries = 4096);
		~CLogPanel() override;

		CLogPanel(const CLogPanel&) = delete;
		CLogPanel& operator=(const CLogPanel&) = delete;

		[[nodiscard]] std::string_view GetName() const override { return "Log"; }
		[[nodiscard]] std::string_view GetMenuName() const override { return "Diagnostics"; }

		void OnAttach() override;
		void OnDetach() override;

		void Render() override;
		void Frame(float _dt) override {};

		void Clear();

		[[nodiscard]] bool GetRenderInMenu() const override { return true; }

	private:

		std::deque<pulvis::core::SLogInfo> m_Entries;
		std::mutex m_EntriesMutex;
		std::size_t m_MaxEntries;
		pulvis::core::log_sink_id_t m_SinkId = pulvis::core::INVALID_LOG_SINK_ID;

		bool m_AutoScroll = true;
		bool m_ShowFatal = true;
		bool m_ShowError = true;
		bool m_ShowWarning = true;
		bool m_ShowInfo = true;
		bool m_ShowDebug = true;
	};
}