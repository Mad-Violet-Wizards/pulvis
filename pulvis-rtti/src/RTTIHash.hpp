#pragma once

#include "RTTITypeTraits.hpp"

namespace pulvis::rtti
{
	constexpr type_id_t Hash(std::string_view _str)
	{
		// FNV-1a hash
		type_id_t hash = 2166136261u;
		for (char c : _str)
		{
			hash ^= static_cast<type_id_t>(c);
			hash *= 16777619u;
		}
		return hash;
	}

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