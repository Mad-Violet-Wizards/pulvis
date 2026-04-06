#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>


namespace pulvis::fs::serialization
{
	enum class EEndian : short
	{
		Little,
		Big
	};

	inline constexpr EEndian GetNativeEndian()
	{
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		return EEndian::Big;
#else
		return EEndian::Little;
#endif
	}

	inline uint16_t ByteSwap16(uint16_t _value)
	{
		return (_value >> 8) | (_value << 8);
	}

	inline uint32_t ByteSwap32(uint32_t _value)
	{
		return ((_value >> 24) & 0x000000FF) |
				 ((_value >> 8) & 0x0000FF00) |
				 ((_value << 8) & 0x00FF0000) |
				 ((_value << 24) & 0xFF000000);
	}

	inline uint64_t ByteSwap64(uint64_t _value)
	{
		return ((_value >> 56) & 0x00000000000000FFULL) |
				 ((_value >> 40) & 0x000000000000FF00ULL) |
				 ((_value >> 24) & 0x0000000000FF0000ULL) |
				 ((_value >> 8) & 0x00000000FF000000ULL) |
				 ((_value << 8) & 0x000000FF00000000ULL) |
				 ((_value << 24) & 0x0000FF0000000000ULL) |
				 ((_value << 40) & 0x00FF000000000000ULL) |
				 ((_value << 56) & 0xFF00000000000000ULL);
	}

	template<typename T>
	T ToLittleEndian(T _value)
	{
		if constexpr (sizeof(T) == 1 || GetNativeEndian() == EEndian::Little)
		{
			return _value;
		}

		uint64_t raw = 0;
		std::memcpy(&raw, &_value, sizeof(T));

		if constexpr (sizeof(T) == 2)
		{
			raw = ByteSwap16(static_cast<uint16_t>(raw));
		}
		else if constexpr (sizeof(T) == 4)
		{
			raw = ByteSwap32(static_cast<uint32_t>(raw));
		}
		else if constexpr (sizeof(T) == 8)
		{
			raw = ByteSwap64(raw);
		}

		T result;
		std::memcpy(&result, &raw, sizeof(T));
		return result;
	}

	template<typename T>
	T FromLittleEndian(T _value)
	{
		return ToLittleEndian(_value);
	}
}