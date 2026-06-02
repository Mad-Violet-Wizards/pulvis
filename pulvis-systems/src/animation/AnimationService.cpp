#include "animation/AnimationService.hpp"
#include "animation/Animation.hpp"
#include "animation/components/AnimationComponent.hpp"
#include "animation/components/AnimationStateComponent.hpp"

#include "EcsWorld.hpp"
#include "Logger.hpp"

#include "src_rtti_autogen.hpp"

namespace pulvis::systems::animation
{
	CAnimationService::CAnimationService()
	{
		RegisterRTTI_src();
	}

	CAnimationService::~CAnimationService()
	{
		if (m_Initialized) { Shutdown(); }
	}

	void CAnimationService::Initialize(pulvis::fs::assets::CAssetRegistry& /*_asset_registry*/)
	{
		if (m_Initialized) 
		{ 
			return; 
		}

		m_Registry = std::make_unique<CAnimationRegistry>();
		m_EventRegistry = std::make_unique<CAnimationEventRegistry>();
		m_EventManager = std::make_unique<CAnimationEventManager>();
		m_Initialized = true;
		PULVIS_INFO_LOG("[AnimationService] Initialized.");
	}

	void CAnimationService::Shutdown()
	{
		if (!m_Initialized) { return; }
		m_EventManager.reset();
		m_EventRegistry.reset();
		m_Registry.reset();
		m_Initialized = false;
		PULVIS_INFO_LOG("[AnimationService] Shutdown.");
	}

	bool CAnimationService::PlaySequence(pulvis::ecs::CWorld& _world,
		pulvis::ecs::SEntityHandle _entity,
		anim_id_t _animation, sequence_id_t _sequence) const
	{
		const SAnimation* anim = m_Registry->FindAnimation(_animation);
		if (anim == nullptr)
		{ 
			return false; 
		}

		const SAnimationSequence* seq = anim->FindSequence(_sequence);
		if (seq == nullptr) 
		{ 
			return false; 
		}

		SAnimationComponent* component = _world.TryGetComponent<SAnimationComponent>(_entity);
		if (!component)
		{
			return false;
		}

		const bool changed = (component->Sequence != seq);

		component->Animation = anim;
		component->Sequence = seq;
		component->AnimationId = _animation;
		component->SequenceId = _sequence;

		if (changed)
		{
			SAnimationStateComponent* state = _world.TryGetComponent<SAnimationStateComponent>(_entity);
			if (state)
			{
				state->ElapsedMs = 0.f;
				state->CurrentFrame = 0;
				state->PrevFrame = 0;
				state->LoopCount = 0;
			}
		}

		return true;
	}
} // namespace pulvis::systems::animation