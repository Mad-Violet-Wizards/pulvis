#pragma once

#include "Animation.hpp"
#include "AnimationTypes.hpp"

#include <memory>
#include <shared_mutex>
#include <unordered_map>

namespace pulvis::systems::animation
{
	class CAnimationRegistry final
	{
	public:

		CAnimationRegistry() = default;
		~CAnimationRegistry() = default;

		CAnimationRegistry(const CAnimationRegistry&) = delete;
		CAnimationRegistry& operator=(const CAnimationRegistry&) = delete;

		anim_id_t Register(std::unique_ptr<SAnimation> _anim);
		bool      Unregister(anim_id_t _id);

		[[nodiscard]] const SAnimation* FindAnimation(anim_id_t _id) const;
		[[nodiscard]] const SAnimationSequence* FindSequence(anim_id_t _anim, sequence_id_t _seq) const;

		void Clear();

	private:

		mutable std::shared_mutex                                  m_Mutex;
		std::unordered_map<anim_id_t, std::unique_ptr<SAnimation>> m_Animations;
	};
} // namespace pulvis::systems::animation