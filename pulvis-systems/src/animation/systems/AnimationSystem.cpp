#include "animation/systems/AnimationSystem.hpp"
#include "animation/AnimationEventManager.hpp"
#include "animation/Animation.hpp"
#include "animation/components/AnimationComponent.hpp"
#include "animation/components/AnimationStateComponent.hpp"

#include "EcsWorld.hpp"
#include "components/SpriteComponent.hpp"
#include "components/DirectionComponent.hpp"

#include <cmath>

namespace pulvis::systems::animation
{
	namespace
	{
		[[nodiscard]] float AdvanceClock(EPlaybackMode _mode,
			float _total_ms,
			float _dt_ms,
			float& _elapsed_ms,
			uint16_t& _loop_count)
		{
			_elapsed_ms += _dt_ms;

			switch (_mode)
			{
			case EPlaybackMode::Single:
			{
				if (_elapsed_ms >= _total_ms)
				{
					_elapsed_ms = _total_ms;
					_loop_count = 1u;
				}
				return _elapsed_ms;
			}

			case EPlaybackMode::Looped:
			{
				while (_elapsed_ms >= _total_ms)
				{
					_elapsed_ms -= _total_ms;
					++_loop_count;
				}
				return _elapsed_ms;
			}

			case EPlaybackMode::PingPong:
			{
				const float cycle = _total_ms * 2.f;
				while (_elapsed_ms >= cycle)
				{
					_elapsed_ms -= cycle;
					++_loop_count;
				}
				return (_elapsed_ms <= _total_ms) ? _elapsed_ms : (cycle - _elapsed_ms);
			}
			}

			return _elapsed_ms;
		}
	} // namespace

	CAnimationSystem::CAnimationSystem(CAnimationEventManager& _event_manager)
		: pulvis::ecs::CSystemBase<SAnimationStateComponent>("AnimationSystem", pulvis::ecs::ESystemPhase::Animation, /*thread_safe=*/true)
		, m_EventManager(_event_manager)
	{
	}

	void CAnimationSystem::Frame(pulvis::ecs::CWorld& _world, float _delta_time)
	{
		auto* state_storage = _world.GetStorage<SAnimationStateComponent>();
		auto* anim_storage = _world.GetStorage<SAnimationComponent>();
		auto* sprite_storage = _world.GetStorage<pulvis::ecs::SSpriteComponent>();

		if (state_storage == nullptr || anim_storage == nullptr || sprite_storage == nullptr)
		{
			return;
		}

		auto* dir_storage = _world.GetStorage<pulvis::ecs::SDirectionComponent>();

		state_storage->ForEach([&](pulvis::ecs::SEntityHandle _entity, SAnimationStateComponent& _state)
			{
				SAnimationComponent* anim = anim_storage->TryGet(_entity);
				pulvis::ecs::SSpriteComponent* sprite = sprite_storage->TryGet(_entity);
				if (anim == nullptr || sprite == nullptr || anim->Animation == nullptr || anim->Sequence == nullptr)
				{
					return;
				}

				const SAnimationSequence& seq = *anim->Sequence;
				if (seq.FrameCount() == 0 || seq.TotalDurationMs <= 0.f)
				{
					return;
				}

				float local_ms = _state.ElapsedMs;
				if (!HasFlag(anim->Flags, EAnimationFlags::Paused))
				{
					const float dt_ms = _delta_time * 1000.f * anim->Speed;
					local_ms = AdvanceClock(seq.Playback, seq.TotalDurationMs, dt_ms, _state.ElapsedMs, _state.LoopCount);
				}

				if (HasFlag(anim->Flags, EAnimationFlags::Reverse))
				{
					local_ms = seq.TotalDurationMs - local_ms;
				}

				_state.PrevFrame = _state.CurrentFrame;
				_state.CurrentFrame = FrameAtTimeMs(seq, local_ms);

				UpdateSprite(*sprite, *anim->Animation, seq.Frames[_state.CurrentFrame], anim->Flags);

				if (_state.CurrentFrame != _state.PrevFrame)
				{
					pulvis::ecs::direction_t direction = pulvis::ecs::DIRECTION_SOUTH;
					if (dir_storage != nullptr)
					{
						if (auto* dir = dir_storage->TryGet(_entity))
						{
							if (dir->Direction != pulvis::ecs::INVALID_DIRECTION) { direction = dir->Direction; }
						}
					}

					for (const SAnimationSequenceEvent& ev : seq.Events)
					{
						if (ev.Frame == _state.CurrentFrame)
						{
							SAnimationFrameContext ctx{};
							ctx.Owner = _entity;
							ctx.AnimationId = anim->AnimationId;
							ctx.SequenceId = anim->SequenceId;
							ctx.EventId = ev.EventId;
							ctx.FrameIndex = _state.CurrentFrame;
							ctx.Direction = direction;
							m_PendingEvents.push_back(ctx);
						}
					}
				}
			});

		for (const SAnimationFrameContext& ctx : m_PendingEvents)
		{
			m_EventManager.Dispatch(ctx);
		}
		m_PendingEvents.clear();
	}

	frame_id_t CAnimationSystem::FrameAtTimeMs(const SAnimationSequence& _seq, float _t_ms) const
	{
		float acc = 0.f;
		const frame_id_t count = _seq.FrameCount();
		for (frame_id_t i = 0; i < count; ++i)
		{
			acc += _seq.Frames[i].Duration;
			if (_t_ms < acc) { return i; }
		}
		return count > 0 ? static_cast<frame_id_t>(count - 1) : 0;
	}

	void CAnimationSystem::UpdateSprite(pulvis::ecs::SSpriteComponent& _sprite, const SAnimation& _anim, const CAnimationFrame& _frame, EAnimationFlags _flags)
	{
		const float tw = _anim.Tileset.TextureSize.x > 0.f ? _anim.Tileset.TextureSize.x : 1.f;
		const float th = _anim.Tileset.TextureSize.y > 0.f ? _anim.Tileset.TextureSize.y : 1.f;

		const float u0 = static_cast<float>(_frame.X) / tw;
		const float v0 = static_cast<float>(_frame.Y) / th;
		const float uw = static_cast<float>(_frame.Width) / tw;
		const float vh = static_cast<float>(_frame.Height) / th;

		if (HasFlag(_flags, EAnimationFlags::FlipX))
		{
			_sprite.UVRect = { u0 + uw, v0, -uw, vh };
		}
		else
		{
			_sprite.UVRect = { u0, v0, uw, vh };
		}

		_sprite.Size = { static_cast<float>(_frame.Width), static_cast<float>(_frame.Height) };
		if (_anim.Tileset.TextureID != 0)
		{
			_sprite.TextureID = _anim.Tileset.TextureID;
		}
	}

} // namespace pulvis::systems::animation