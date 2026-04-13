#pragma once

#include <cstdint>
#include <type_traits>
#include <algorithm>

namespace pulvis::core::math
{
    template<typename T>
    constexpr T Clamp(T _value, T _min, T _max)
    {
        static_assert(std::is_arithmetic_v<T>, "Clamp function requires an arithmetic type.");
		return (_value < _min) ? _min : ((_value > _max) ? _max : _value);
    }

    template<typename T>
    constexpr T Lerp(T _start, T _end, float _t)
    {
        static_assert(std::is_arithmetic_v<T>, "Lerp function requires an arithmetic type.");
        return _start + (_end - _start) * Clamp(_t, 0.0f, 1.0f);
    }

    template<typename T>
    constexpr float Normalize(T _value, T _min, T _max)
    {
        static_assert(std::is_arithmetic_v<T>, "Normalize function requires an arithmetic type.");
        if (_max - _min == 0)
        {
            return 0.0f;
        }

		return static_cast<float>(_value - _min) / static_cast<float>(_max - _min);
    }

    template<typename T>
    constexpr T Remap(T _value, T _from_min, T _from_max, T _to_min, T _to_max)
    {
        static_assert(std::is_arithmetic_v<T>, "Remap function requires an arithmetic type.");
        if (_from_max - _from_min == 0)
        {
            return _to_min;
        }

        const float normalized = Normalize(_value, _from_min, _from_max);
        return Lerp(_to_min, _to_max, normalized);
    }

    template<typename T>
    constexpr T InverseLerp(T _start, T _end, T _value)
    {
        static_assert(std::is_arithmetic_v<T>, "InverseLerp function requires an arithmetic type.");
		if (_start == _end)
        {
            return 0.f;
        }
        
        return Clamp(static_cast<float>(_value - _start) / static_cast<float>(_end - _start), 0.0f, 1.0f);
    }

    constexpr float SmoothStep(float _edge0, float _edge1, float _x)
    {
        const float t = Clamp((_x - _edge0) / (_edge1 - _edge0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }

    constexpr float SmootherStep(float _edge0, float _edge1, float _x)
    {
        const float t = Clamp((_x - _edge0) / (_edge1 - _edge0), 0.0f, 1.0f);
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }
}