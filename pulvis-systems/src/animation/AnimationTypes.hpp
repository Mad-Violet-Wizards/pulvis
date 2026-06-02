#pragma once

#include "Hash.hpp"

#include <cstdint>
#include <string_view>

namespace pulvis::systems::animation
{
	using anim_id_t = uint32_t;
	using sequence_id_t = uint32_t;
	using event_id_t = uint32_t;
	using frame_id_t = uint16_t;

	constexpr anim_id_t     INVALID_ANIM = 0;
	constexpr sequence_id_t INVALID_SEQUENCE = 0;
	constexpr event_id_t    INVALID_EVENT = 0xFFFFFFFFu;

	[[nodiscard]] constexpr anim_id_t MakeAnimId(std::string_view _name) noexcept
	{
		return pulvis::tl::hash::fnv1a<anim_id_t>(_name);
	}

	[[nodiscard]] constexpr sequence_id_t MakeSequenceId(std::string_view _name) noexcept
	{
		return pulvis::tl::hash::fnv1a<sequence_id_t>(_name);
	}

	enum class EPlaybackMode : uint8_t
	{
		Single = 0,
		Looped = 1,
		PingPong = 2
	};

	enum class EAnimationFlags : uint8_t
	{
		None = 0,
		Paused = 1 << 0,
		Reverse = 1 << 1,
		FlipX = 1 << 2
	};

	[[nodiscard]] constexpr EAnimationFlags operator|(EAnimationFlags _a, EAnimationFlags _b) noexcept
	{
		return static_cast<EAnimationFlags>(static_cast<uint8_t>(_a) | static_cast<uint8_t>(_b));
	}

	[[nodiscard]] constexpr bool HasFlag(EAnimationFlags _set, EAnimationFlags _flag) noexcept
	{
		return (static_cast<uint8_t>(_set) & static_cast<uint8_t>(_flag)) != 0;
	}
} // namespace pulvis::systems::animation