#pragma once

#include <functional>

#include "MathConstans.hpp"

namespace engine::math
{
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	concept Arithmetic = requires(T a, T b)
	{
		{ a + b } -> std::convertible_to<T>;
		{ a - b } -> std::convertible_to<T>;
		{ a* b } -> std::convertible_to<T>;
		{ a / b } -> std::convertible_to<T>;
		{ -a }    -> std::convertible_to<T>;
	};

	//////////////////////////////////////////////////////////////////////////
	template<Arithmetic T>
	constexpr T NormalizeRadians(T _radians)
	{
		return ((_radians + PI) % TWO_PI) - PI;
	}
	//////////////////////////////////////////////////////////////////////////
	template<Arithmetic T>
	constexpr T DegreesToRadians(T _degrees)
	{
		return _degrees * DEG_TO_RAD;
	}

	template<Arithmetic T>
	constexpr T RadiansToDegrees(T _radians)
	{
		return _radians * RAD_TO_DEG;
	}
}
