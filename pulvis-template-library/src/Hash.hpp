#pragma once

#include <cstdint>
#include <string_view>
#include <type_traits>

namespace pulvis::tl
{
	/* 
		Compile time FNV-1a hash. 
		Supports any unsigned integral return type (uint32_t, uint64_t).

		Typical usage:
			constexpr uint64_t hash = hash::FNV1a<uint64_t>("Hello, World!");
	*/
	namespace hash
	{
		namespace detail
		{
			template<typename T>
			struct fnv1a_traits;

			template<>
			struct fnv1a_traits<uint32_t>
			{
				static constexpr uint32_t OffsetBasis = 2166136261u;
				static constexpr uint32_t Prime = 16777619u;
			};

			template<>
			struct fnv1a_traits<uint64_t>
			{
				static constexpr uint64_t OffsetBasis = 14695981039346656037ull;
				static constexpr uint64_t Prime = 1099511628211ull;
			};
		}

		template<typename T = uint32_t>
		constexpr T fnv1a(std::string_view str)
		{
			static_assert(std::is_unsigned_v<T>, "Hash type must be an unsigned integral type.");
			auto traits = detail::fnv1a_traits<T>{};

			T hash = traits.OffsetBasis;
			for (char c : str)
			{
				hash ^= static_cast<T>(c);
				hash *= traits.Prime;
			}

			return hash;
		}

		template<typename T = uint32_t>
		constexpr T fnv1a(const char* str)
		{
			return fnv1a<T>(std::string_view(str));
		}
	}
}