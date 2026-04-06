#pragma once

#include "ThreadTypeTraits.hpp"
#include "Job.hpp"
#include "JobAllocator.hpp"
#include "StealingDequeue.hpp"

#include <thread>
#include <atomic>

namespace pulvis::threads
{
	/*
		Single worker thread in job system.
		Each worker owns:
			* A StealingDequeue -> for its local job queue.
			* A CJobAllocator -> for zero-malloc job creation.
	
		The worker runs a lopp that:
		1. Tries to pop a job from its local queue.
		2. If no local job, tries to steal from other workers.
		3. If no jobs available, backs off with _mm_pause and retries.

		Workers are created and managed by CJobSystem.
	*/

	class CJobSystem;

	class CJobWorker
	{

		public:

			CJobWorker();
			~CJobWorker();

			CJobWorker(const CJobWorker&) = delete;
			CJobWorker& operator=(const CJobWorker&) = delete;

			CJobWorker(CJobWorker&& _other);
			CJobWorker& operator=(CJobWorker&& _other);

			void Start(CJobSystem* _jobSystem, thread_id_t _id);
			void RequestStop();
			void Join();

			bool Submit(SJob* _job);

			SJob* StealJob();

			SJob* AllocateJob();
			void ResetAllocator();

			thread_id_t GetId() const { return m_Id; }
			bool IsRunning() const { return m_Running.load(std::memory_order_acquire); }
			int64_t GetQueueSize() const { return m_Dequeue.Size(); }

		private:
			
			void Run();
			void ExecuteJob(SJob* _job);

		private:

			pulvis::tl::StealingDequeue<SJob*> m_Dequeue;
			CJobAllocator<> m_Allocator;
			CJobSystem* m_JobSystem = nullptr;
			std::thread m_Thread;
			thread_id_t m_Id = 0;
			std::atomic<bool> m_Running = false;

	};
}