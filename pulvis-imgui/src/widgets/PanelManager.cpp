#include "PanelManager.hpp"

namespace pulvis::imgui::widgets
{
	CPanelManager::~CPanelManager()
	{
		Clear();
	}

	panel_id_t CPanelManager::Register(std::unique_ptr<IPanel> _panel)
	{
		const auto id = m_NextPanelID++;
		_panel->OnAttach();
		m_Entries.push_back({ id, std::move(_panel) });
		return id;
	}

	bool CPanelManager::Unregister(panel_id_t _panel_id)
	{
		for (auto it = m_Entries.begin(); it != m_Entries.end(); ++it)
		{
			if (it->ID == _panel_id)
			{
				it->Panel->OnDetach();
				it = m_Entries.erase(it);
				return true;
			}
		}
		return false;
	}

	void CPanelManager::Clear()
	{
		for (auto& entry : m_Entries)
		{
			entry.Panel->OnDetach();
		}
		m_Entries.clear();
	}

	IPanel* CPanelManager::Find(std::string_view _name) const
	{
		for (const auto& entry : m_Entries)
		{
			if (entry.Panel->GetName() == _name)
			{
				return entry.Panel.get();
			}
		}
		return nullptr;

	}

	IPanel* CPanelManager::Find(panel_id_t _panel_id) const
	{
		for (const auto& entry : m_Entries)
		{
			if (entry.ID == _panel_id)
			{
				return entry.Panel.get();
			}
		}

		return nullptr;
	}

	void CPanelManager::Frame(float _dt)
	{
		for (const auto& entry : m_Entries)
		{
			if (entry.Panel->IsVisible())
			{
				entry.Panel->Frame(_dt);
			}
		}
	}

	void CPanelManager::Render()
	{
		for (const auto& entry : m_Entries)
		{
			if (entry.Panel->IsVisible())
			{
				entry.Panel->Render();
			}
		}
	}

	void CPanelManager::ForEach(const std::function<void(panel_id_t, IPanel&)>& _fn) const
	{
		for (const auto& entry : m_Entries)
		{
			_fn(entry.ID, *entry.Panel);
		}
	}
}