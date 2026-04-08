#pragma once

#include <cstdint>
#include <functional>
#include <format>
#include <string>
#include <string_view>

#include "Hash.hpp"

namespace pulvis::level
{
	struct SChunkCoord
	{
		int32_t X;
		int32_t Y;

		bool operator==(const SChunkCoord& other) const
		{
			return X == other.X && Y == other.Y;
		}

		bool operator!=(const SChunkCoord& other) const
		{
			return !(*this == other);
		}

		uint64_t ToHash() const
		{
			const auto* bytes = reinterpret_cast<const char*>(this);
			return pulvis::tl::hash::fnv1a<uint64_t>(std::string_view(bytes, sizeof(SChunkCoord)));		
		}

		std::string ToFileString() const
		{
			return std::format("{:016x}", ToHash());
		}
	};
}

template<>
struct std::hash<pulvis::level::SChunkCoord>
{
	size_t operator()(const pulvis::level::SChunkCoord& _coord) const noexcept
	{
		size_t h1 = std::hash<int32_t>{}(_coord.X);
		size_t h2 = std::hash<int32_t>{}(_coord.Y);
		return h1 ^ (h2 << 1);
	}
};