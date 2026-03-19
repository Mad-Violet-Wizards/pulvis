#pragma once

#include <cstdint>

namespace pulvis::threads
{
	enum class EJobPriority : uint8_t
	{
		Low = 0,
		Normal = 1,
		High = 2,
		Critical = 3
	};
}