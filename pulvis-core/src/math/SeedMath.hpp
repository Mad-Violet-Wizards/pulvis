#pragma once

#include "MathUtils.hpp"

#include <cstdint>

namespace pulvis::core::math
{
    namespace detail
    {
        constexpr uint64_t SPLIT_MIX_INCREMENT = 0x9E3779B97F4A7C15ULL;
		constexpr uint64_t SPLIT_MIX_MUL1      = 0xBF58476D1CE4E5B9ULL;
		constexpr uint64_t SPLIT_MIX_MUL2      = 0x94D049BB133111EBULL;
		constexpr uint64_t DERIVE_PRIME        = 0x517CC1B727220A95ULL;
    }

	inline uint64_t SplitMix64(uint64_t _state)
	{
		_state += detail::SPLIT_MIX_INCREMENT;
		_state = (_state ^ (_state >> 30)) * detail::SPLIT_MIX_MUL1;
		_state = (_state ^ (_state >> 27)) * detail::SPLIT_MIX_MUL2;
		return _state ^ (_state >> 31);
	}

    inline uint64_t DeriveSeed(uint64_t _seed, uint64_t _salt)
    {
        return SplitMix64(_seed ^ (_salt * detail::DERIVE_PRIME));
    }

	inline uint64_t DeriveSeed(uint64_t _parentSeed, int32_t _x, int32_t _y)
	{
		const uint64_t combined = (
            static_cast<uint64_t>(static_cast<uint32_t>(_x)) << 32) | 
            static_cast<uint64_t>(static_cast<uint32_t>(_y)
        );

		return DeriveSeed(_parentSeed, combined);
	}

	inline float NormalizedFloat(uint64_t _seed)
    {
		const uint64_t hash = SplitMix64(_seed);
		return Normalize(static_cast<uint32_t>(hash & 0xFFFFFFFF),
			static_cast<uint32_t>(0), static_cast<uint32_t>(0xFFFFFFFF));  
    }

    inline float RangeFloat(uint64_t _seed, float _min, float _max)
    {
        return Lerp(_min, _max, NormalizedFloat(_seed));
    }

    inline int32_t RangeInt(uint64_t _seed, int32_t _min, int32_t _max)
    {
		if (_min >= _max) return _min;
		const uint64_t hash = SplitMix64(_seed);
		return _min + static_cast<int32_t>(hash % static_cast<uint64_t>(_max - _min + 1));   
    }
}