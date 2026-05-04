#pragma once

#include "Panel.hpp"

namespace pulvis::imgui::widgets
{
	class CStatsPanel final : public IPanel
	{
		public:
			[[nodiscard]] std::string_view GetName() const override { return "Stats"; }
			[[nodiscard]] std::string_view GetMenuName() const override { return "Diagnostics"; }

			void OnAttach() override {};
			void OnDetach() override {};

			void Frame(float _dt) override;
			void Render() override;

			bool GetRenderInMenu() const override { return true; }

		private:

			float m_Accumulator = 0.0f;
			float m_FPS = 0.0f;
			float m_FrameTimeMs = 0.0f;
			int m_FrameCounter = 0;
	};
}