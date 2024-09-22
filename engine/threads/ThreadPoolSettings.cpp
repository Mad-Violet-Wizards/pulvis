#include "engine/engine_pch.hpp"
#include "ThreadPoolSettings.hpp"

namespace engine::threads
{
	SThreadPoolSettings::SThreadPoolSettings()
		: m_NumThreads(std::thread::hardware_concurrency())
		, m_QueueSize(S_DEFAULT_QUEUE_SIZE)
	{
	}
}
