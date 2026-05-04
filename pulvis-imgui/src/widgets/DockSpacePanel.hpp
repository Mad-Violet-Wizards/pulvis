#pragma once

#include "Panel.hpp"

namespace pulvis::imgui::widgets
{
	class CDockSpacePanel final : public IPanel
	{
		public:

			[[nodiscard]] std::string_view GetName() const override { return "DockSpace"; }
			[[nodiscard]] std::string_view GetMenuName() const override { return "None"; }

			void OnAttach() override {};
			void OnDetach() override {};
			void Frame(float _dt) override {};
			void Render() override;

			bool GetRenderInMenu() const override { return false; }
	};
}