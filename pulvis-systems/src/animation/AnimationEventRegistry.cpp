#include "AnimationEventRegistry.hpp"

#include "Logger.hpp"

#include <utility>

namespace pulvis::systems::animation
{
	bool CAnimationEventRegistry::Register(SAnimationEvent _event) noexcept
	{
		if (_event.ID == INVALID_EVENT)
		{
			PULVIS_ERROR_LOG("[AnimationEventRegistry] Refused to register event with INVALID_EVENT id (name='{}').", _event.Name);
			return false;
		}
		if (_event.Name.empty())
		{
			PULVIS_ERROR_LOG("[AnimationEventRegistry] Refused to register event with empty name (id={}).", _event.ID);
			return false;
		}

		if (m_NameToId.find(_event.Name) != m_NameToId.end())
		{
			PULVIS_ERROR_LOG("[AnimationEventRegistry] Duplicate event name '{}'.", _event.Name);
			return false;
		}
		if (m_IdToIndex.find(_event.ID) != m_IdToIndex.end())
		{
			PULVIS_ERROR_LOG("[AnimationEventRegistry] Duplicate event id {} (incoming name='{}').", _event.ID, _event.Name);
			return false;
		}

		const uint32_t index = static_cast<uint32_t>(m_Events.size());
		m_NameToId.emplace(_event.Name, _event.ID);
		m_IdToIndex.emplace(_event.ID, index);
		m_Events.push_back(std::move(_event));
		return true;
	}

	event_id_t CAnimationEventRegistry::FindId(std::string_view _name) const noexcept
	{
		const auto it = m_NameToId.find(std::string(_name));
		return it != m_NameToId.end() ? it->second : INVALID_EVENT;
	}

	const SAnimationEvent* CAnimationEventRegistry::Find(event_id_t _id) const noexcept
	{
		const auto it = m_IdToIndex.find(_id);
		return it != m_IdToIndex.end() ? &m_Events[it->second] : nullptr;
	}

	const std::string* CAnimationEventRegistry::FindName(event_id_t _id) const noexcept
	{
		const auto it = m_IdToIndex.find(_id);
		return it != m_IdToIndex.end() ? &m_Events[it->second].Name : nullptr;
	}

	void CAnimationEventRegistry::Clear()
	{
		m_Events.clear();
		m_NameToId.clear();
		m_IdToIndex.clear();
	}
} // namespace pulvis::systems::animation