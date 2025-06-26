#pragma once

#include "engine/experimental/memory/MemoryCategory.hpp"
#include "engine/experimental/memory/MemoryAlign.hpp"

namespace tests::experimental::memory
{
	constexpr static size_t BENCHMARK_ITERATIONS = 512;
	constexpr static size_t ALLOCATOR_BENCHMARK_SIZE = 2048;

	class align1 OneByteClass
	{
		public:
			OneByteClass()
			{
				m_Value[0] = std::byte(0);
			}

			OneByteClass(std::byte _value)
			{
				m_Value[0] = _value;
			}

		std::byte m_Value[1];
	};

	class align2 TwoByteClass
	{
		std::byte m_Value[2];
	};

	class ThreeByteClass
	{
		std::byte m_Value[3];
	};

	class align4 FourByteClass
	{
		std::byte m_Value[4];
	};

	class align8 EightByteClass
	{
		std::byte m_Value[8];
	};

	class align16 SixteenByteClass
	{
		std::byte m_Value[16];
	};

	class align32 ThirtyTwoByteClass
	{
		std::byte m_Value[32];
	};

	class FourtyTwoByteClass
	{
		std::byte m_Value[42];
	};

	class align64 SixtyFourByteClass
	{
		std::byte m_Value[64];
	};

	class align128 OneTwentyEightByteClass
	{
		std::byte m_Value[128];
	};
}