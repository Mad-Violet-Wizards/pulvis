#pragma once

#include "Utils.hpp"

namespace engine::threads
{
	static constexpr concurrency_t S_DEFAULT_QUEUE_SIZE = 1024;

	struct PULVIS_API SThreadPoolSettings
	{
		SThreadPoolSettings();

		concurrency_t m_NumThreads;
		concurrency_t m_QueueSize;
	};
}