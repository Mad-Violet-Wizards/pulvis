#include "AnimationEventManager.hpp"
#include "AnimationEventController.hpp"

#include <algorithm>

namespace pulvis::systems::animation
{
	void CAnimationEventManager::RegisterController(event_id_t _event_id, IAnimationEventController* _controller)
	{
		if (_controller == nullptr || _event_id == INVALID_EVENT)
		{
			return;
		}
		auto& list = m_Subscriptions[_event_id];
		if (std::find(list.begin(), list.end(), _controller) == list.end())
		{
			list.push_back(_controller);
		}
	}

	void CAnimationEventManager::UnregisterController(IAnimationEventController* _controller)
	{
		if (_controller == nullptr)
		{
			return;
		}
		for (auto it = m_Subscriptions.begin(); it != m_Subscriptions.end(); )
		{
			auto& list = it->second;
			list.erase(std::remove(list.begin(), list.end(), _controller), list.end());
			if (list.empty())
			{
				it = m_Subscriptions.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void CAnimationEventManager::Dispatch(const SAnimationFrameContext& _context) const
	{
		const auto it = m_Subscriptions.find(_context.EventId);
		if (it == m_Subscriptions.end())
		{
			return;
		}
		for (IAnimationEventController* controller : it->second)
		{
			controller->OnAnimationEvent(_context);
		}
	}
} // namespace pulvis::systems::animation