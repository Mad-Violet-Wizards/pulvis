#pragma once

#include "ThreadTypeTraits.hpp"
#include "Job.hpp"
#include "JobCounter.hpp"
#include "JobWorker.hpp"
#include "StealingDequeue.hpp"

#include <vector>
#include <memory>

namespace pulvis::threads
{
	struct SJobSystemSettings
	{
		thread_id_t ThreadsCount = 0;
		size_t DequeueCapacity = DEFAULT_DEQUEUE_CAPACITY;
	};

	/*
	Manages worker threads and provides the public API for scheduling, waiting, and shutdown.

	The main thread can participate in work by calling HelpWithWork()
	while waiting on a counter, preventing it from sitting idle.

	Typical usage:
		CJobSystem jobSystem;
		jobSystem.Initialize();

		CJobCounter counter;
		SJob* job = jobSystem.CreateJob(
			[](SJob* _self) { do_physics(); },
			&counter
		);
		jobSystem.Submit(job);
		jobSystem.Wait(&counter);

		jobSystem.Shutdown();
*/

	class CJobSystem
	{
		public:

			CJobSystem();
			~CJobSystem();

			CJobSystem(const CJobSystem&) = delete;
			CJobSystem& operator=(const CJobSystem&) = delete;

			void Initialize(const SJobSystemSettings& _settings = {});

			void Shutdown();

			template<typename Func>
			SJob* CreateJob(Func&& _func, CJobCounter* _counter = nullptr, EJobPriority _priority = EJobPriority::Normal)
			{
				thread_id_t worker_idx = GetNextWorkerIndex();
				SJob* job = m_Workers[worker_idx]->AllocateJob();

				job->FastFunction = pulvis::tl::FastFunction::Make<void, SJob*>(std::forward<Func>(_func));
				job->Counter = _counter;
				job->Priority = _priority;

				if (_counter)
				{
					(*_counter) += 1;
				}

				return job;
			}

			void Submit(SJob* _job);
			void SubmitBatch(SJob** _jobs, size_t _count);

			void Wait(CJobCounter* _counter);

			bool HelpWithWork();
			SJob* StealFromSubmitQueue();
			SJob* StealFromRandom(thread_id_t _thiefId);
			SJob* StealFromBusiest(thread_id_t _thiefId);

			void ResetAllocators();
			thread_id_t GetWorkerCount() const { return static_cast<thread_id_t>(m_Workers.size()); }
			bool IsInitialized() const { return m_Initialized; }

		private:

			thread_id_t GetNextWorkerIndex();

		private:

			std::vector<std::unique_ptr<CJobWorker>> m_Workers;
			tl::StealingDequeue<SJob*> m_SubmitQueue;
			std::atomic<thread_id_t> m_NextWorkerIndex{ 0 };
			bool m_Initialized = false;
	};
}