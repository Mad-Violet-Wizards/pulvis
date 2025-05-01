#pragma once

#include <cstdint>

namespace engine::pstd
{
	constexpr uint32_t ConstexprStringHash(const char* _str)
	{
		// FNV-1a hash
		uint32_t hash = 2166136261u;
		while (*_str)
		{
			hash ^= *_str++;
			hash *= 16777619u;
		}
		return hash;
	}
}