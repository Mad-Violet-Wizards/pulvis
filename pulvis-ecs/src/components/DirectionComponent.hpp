#pragma once

#include <cstdint>

namespace pulvis::ecs
{
	using direction_t = uint8_t;

	enum class EDirectionMode : uint8_t
	{
		None = 0,
		Single = 1, // entity ignores direction, single set of frames
		Cardinal = 2, // (N, E, S, W)
		Ordinal = 3  // (NE, SE, SW, NW + N, E, S, W)
	};

	constexpr direction_t INVALID_DIRECTION = 0xFF;
	constexpr direction_t DIRECTION_NORTH = 0x00;
	constexpr direction_t DIRECTION_NORTH_EAST = 0x01;
	constexpr direction_t DIRECTION_EAST = 0x02;
	constexpr direction_t DIRECTION_SOUTH_EAST = 0x03;
	constexpr direction_t DIRECTION_SOUTH = 0x04;
	constexpr direction_t DIRECTION_SOUTH_WEST = 0x05;
	constexpr direction_t DIRECTION_WEST = 0x06;
	constexpr direction_t DIRECTION_NORTH_WEST = 0x07;

	struct SDirectionComponent
	{
		SDirectionComponent() = default;
		SDirectionComponent(EDirectionMode _mode, direction_t _direction)
			: Mode(_mode), Direction(_direction) {
		}

		EDirectionMode Mode = EDirectionMode::None;
		direction_t    Direction = INVALID_DIRECTION;
	};
}