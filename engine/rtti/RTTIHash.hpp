#pragma once

#include "RTTITypeTraits.hpp"

namespace engine::rtti
{
	constexpr type_id_t Hash(const char* _str)
	{
		// FNV-1a hash
		type_id_t hash = 2166136261u;
		while (*_str)
		{
			hash ^= *_str++;
			hash *= 16777619u;
		}
		return hash;
	}
}