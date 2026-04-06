#pragma once

#include "Job.hpp"
#include "ThreadTypeTraits.hpp"

#include <array>
#include <atomic>

namespace pulvis::threads
{
	/*
			Lock-free pool allocator for SJob instances.

		Pre-allocates a fixed-size ring of cache-line-aligned jobs
		and hands them out via an atomic index. Each worker thread
		should own its own CJobAllocator to avoid contention.

		Allocation is O(1) and never touches the heap at runtime.
		When the pool wraps around, slots are reused. The caller must
		ensure jobs are completed before their slots are reclaimed
		(guaranteed by frame-based scheduling with Wait()).

		Typical usage:
			CJobAllocator<4096> allocator;
			SJob* job = allocator.Allocate(); // zero-initialized slot
			job->m_Function = ...;
			// ... submit job ...
			allocator.Reset(); // call once per frame
	*/

	template<size_t PoolSize = DEFAULT_JOB_POOL_SIZE>
	class CJobAllocator
	{
	public:

		CJobAllocator()
			: m_Head(0) {}

		CJobAllocator(const CJobAllocator&) = delete;
		CJobAllocator& operator=(const CJobAllocator&) = delete;

		SJob* Allocate()
		{
			const size_t index = m_Head.fetch_add(1, std::memory_order_relaxed) % PoolSize;
			SJob& job = m_JobPool[index];
			job.FastFunction.Reset();
			job.Counter = nullptr;
			job.Priority = EJobPriority::Normal;
			return &job;
		}

		void Reset()
		{
			m_Head.store(0, std::memory_order_relaxed);
			for (auto& job : m_JobPool)
			{
				job.FastFunction.Reset();
				job.Counter = nullptr;
				job.Priority = EJobPriority::Normal;
			}
	}

	private:

		alignas(CACHE_LINE_SIZE) std::array<SJob, PoolSize> m_JobPool;
		std::atomic<size_t> m_Head;
	};
}