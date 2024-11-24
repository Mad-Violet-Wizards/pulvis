#pragma once

#include "engine/memory/MemoryCategory.hpp"
#include "engine/memory/MemoryAlign.hpp"

namespace engine::memory::tests
{
	class align1 OneByteClass
	{
		std::byte m_Value[1];
	};

	class align2 TwoByteClass
	{
		std::byte m_Value[2];
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

	class align64 SixtyFourByteClass
	{
		std::byte m_Value[64];
	};

	class align128 OneTwentyEightByteClass
	{
		std::byte m_Value[128];
	};
}