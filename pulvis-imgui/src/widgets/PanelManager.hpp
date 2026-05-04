#pragma once

#include "Panel.hpp"
#include <cstddef>
#include <functional>
#include <memory>
#include <string_view>
#include <vector>

namespace pulvis::imgui::widgets
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	using panel_id_t = std::size_t;
	constexpr static panel_id_t INVALID_PANEL_ID = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
	class CPanelManager final
	{
		public:

			CPanelManager() = default;
			~CPanelManager();

			CPanelManager(const CPanelManager&) = delete;
			CPanelManager& operator=(const CPanelManager&) = delete;

			panel_id_t Register(std::unique_ptr<IPanel> _panel);
			bool Unregister(panel_id_t _panel_id);
			void Clear();

			[[nodiscard]] IPanel* Find(std::string_view _name) const;
			[[nodiscard]] IPanel* Find(panel_id_t _panel_id) const;

			template<typename T>
			[[nodiscard]] T* FindAs(std::string_view _name) const
			{
				return dynamic_cast<T*>(Find(_name));
			}

			void Frame(float _dt);
			void Render();

			void ForEach(const std::function<void(panel_id_t, IPanel&)>& _fn) const;

			[[nodiscard]] std::size_t Count() const { return m_Entries.size(); }

		private:

			struct SEntry
			{
				panel_id_t ID;
				std::unique_ptr<IPanel> Panel;
			};

			std::vector<SEntry> m_Entries;
			panel_id_t m_NextPanelID = 1;
	};
}