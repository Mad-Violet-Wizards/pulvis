#include "AnimationRegistry.hpp"

#include "Logger.hpp"

namespace pulvis::systems::animation
{
	anim_id_t CAnimationRegistry::Register(std::unique_ptr<SAnimation> _anim)
	{
		if (!_anim)
		{
			return INVALID_ANIM;
		}

		std::unique_lock lock(m_Mutex);

		const anim_id_t id = _anim->Id;
		auto [it, inserted] = m_Animations.try_emplace(id, std::move(_anim));
		if (!inserted)
		{
			PULVIS_WARNING_LOG("[AnimationRegistry] Animation id collision for '{}'. Keeping existing entry.", it->second->Name);
			return INVALID_ANIM;
		}

		return id;
	}

	bool CAnimationRegistry::Unregister(anim_id_t _id)
	{
		std::unique_lock lock(m_Mutex);
		return m_Animations.erase(_id) > 0;
	}

	const SAnimation* CAnimationRegistry::FindAnimation(anim_id_t _id) const
	{
		std::shared_lock lock(m_Mutex);
		const auto it = m_Animations.find(_id);
		return it != m_Animations.end() ? it->second.get() : nullptr;
	}

	const SAnimationSequence* CAnimationRegistry::FindSequence(anim_id_t _anim, sequence_id_t _seq) const
	{
		const SAnimation* anim = FindAnimation(_anim);
		return anim ? anim->FindSequence(_seq) : nullptr;
	}

	void CAnimationRegistry::Clear()
	{
		std::unique_lock lock(m_Mutex);
		m_Animations.clear();
	}
} // namespace pulvis::systems::animation