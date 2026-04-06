#pragma once

#include "ThreadTypeTraits.hpp"
#include "JobPriority.hpp"
#include "JobCounter.hpp"

#include "FastFunction.hpp"

#include <cstdint>
#include <cstring>

namespace pulvis::threads
{
	/*
	Job structure aligned to 64-byte cache line.

		Uses pulvis::tl::FastFunction (24B inline buffer) to store
		the callable - supports lambdas, free functions, and member
		function pointers without heap allocation.

		Dependency tracking is done via a shared CJobCounter pointer.
		The job system decrements the counter after execution, allowing
		dependent jobs to proceed once the counter reaches zero.

		Typical usage:
			SJob job;
			job.m_Function = FastFunction::Make<void, SJob*>(
				[](SJob* _self) { do_work(); }
			);
			job.m_Counter = &myCounter;
			jobSystem.Submit(&job);
	*/

	struct alignas(CACHE_LINE_SIZE) SJob
	{
		pulvis::tl::FastFunction FastFunction; // 48 bytes.
		CJobCounter* Counter = nullptr; // 8 bytes 

		EJobPriority Priority = EJobPriority::Normal; // 1 byte.

		std::byte Padding[7]; // Padding to ensure 64-byte alignment.
	};

	static_assert(sizeof(SJob) == CACHE_LINE_SIZE, "SJob must be 64 bytes to fit in a single cache line.");
	static_assert(sizeof(SJob) % CACHE_LINE_SIZE == 0, "SJob size must be a multiple of cache line size.");
}