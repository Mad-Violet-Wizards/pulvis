#pragma once

#include <cstdint>

namespace pulvis::level
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	struct alignas(8) STile
	{
		uint16_t TilesetIndex = 0;
		uint16_t TileIndex = 0;
		uint8_t Flags = 0;
		uint8_t AutotileMask = 0;
		uint8_t Variant = 0;
		uint8_t Reserved = 0;
	};

	static_assert(sizeof(STile) == 8, "STile must be 8 bytes in size.");
	constexpr STile EMPTY_TILE = { 0xFFFF, 0, 0, 0, 0, 0 };

///////////////////////////////////////////////////////////////////////////////////////////////////
	enum class ETileFlag : uint8_t
	{
		None = 0,
		Solid = 1,
		Destructible = 2,
		Interactable = 4,
		Trigger = 8,
		Liquid = 16,
		Animated = 32,
		Opaque = 64
	};

	constexpr uint8_t operator|(ETileFlag a, ETileFlag b)
	{
		return static_cast<uint8_t>(a) | static_cast<uint8_t>(b);
	}

	constexpr uint8_t operator&(ETileFlag a, ETileFlag b)
	{
		return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
	}

	constexpr uint8_t operator~(ETileFlag a)
	{
		return ~static_cast<uint8_t>(a);
	}

	constexpr uint8_t operator|(uint8_t a, ETileFlag b)
	{
		return a | static_cast<uint8_t>(b);
	}

	constexpr uint8_t operator&(uint8_t a, ETileFlag b)
	{
		return a & static_cast<uint8_t>(b);
	}

	constexpr uint8_t operator|=(uint8_t& a, ETileFlag b)
	{
		a |= static_cast<uint8_t>(b);
		return a;
	}

	constexpr uint8_t operator&=(uint8_t& a, ETileFlag b)
	{
		a &= static_cast<uint8_t>(b);
		return a;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////
}