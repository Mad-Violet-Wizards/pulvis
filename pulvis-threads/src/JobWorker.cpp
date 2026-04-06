#include "JobWorker.hpp"
#include "JobSystem.hpp"

#include <random>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace pulvis::threads
{
	CJobWorker::CJobWorker()
		: m_Dequeue(DEFAULT_DEQUEUE_CAPACITY)
	{
	}

	CJobWorker::~CJobWorker()
	{
		if (IsRunning())
		{
			RequestStop();
			Join();
		}
	}

	CJobWorker::CJobWorker(CJobWorker&& _other)
		: m_Dequeue(std::move(_other.m_Dequeue))
		, m_JobSystem(_other.m_JobSystem)
		, m_Thread(std::move(_other.m_Thread))
		, m_Id(_other.m_Id)
		, m_Running(_other.m_Running.load(std::memory_order_relaxed))
	{
		_other.m_JobSystem = nullptr;
		_other.m_Id = 0;
		_other.m_Running.store(false, std::memory_order_relaxed);
	}

	CJobWorker& CJobWorker::operator=(CJobWorker&& _other)
	{
		if (this != &_other)
		{
			m_Dequeue = std::move(_other.m_Dequeue);
			m_JobSystem = _other.m_JobSystem;
			m_Thread = std::move(_other.m_Thread);
			m_Id = _other.m_Id;
			m_Running.store(_other.m_Running.load(std::memory_order_relaxed), std::memory_order_relaxed);

			_other.m_JobSystem = nullptr;
			_other.m_Id = 0;
			_other.m_Running.store(false, std::memory_order_relaxed);
		}

		return *this;
	}

	void CJobWorker::Start(CJobSystem* _jobSystem, thread_id_t _id)
	{
		m_Id = _id;
		m_JobSystem = _jobSystem;
		m_Running.store(true, std::memory_order_release);
		m_Thread = std::thread(&CJobWorker::Run, this);
	}

	void CJobWorker::RequestStop()
	{
		m_Running.store(false, std::memory_order_release);
	}

	void CJobWorker::Join()
	{
		if (m_Thread.joinable())
		{
			m_Thread.join();
		}
	}

	bool CJobWorker::Submit(SJob* _job)
	{
		return m_Dequeue.Push(_job);
	}

	SJob* CJobWorker::StealJob()
	{
		return m_Dequeue.Steal();
	}

	SJob* CJobWorker::AllocateJob()
	{
		return m_Allocator.Allocate();
	}

	void CJobWorker::ResetAllocator()
	{
		m_Allocator.Reset();
	}

	void CJobWorker::Run()
	{
		// Thread local RNG for randomized stealing.
		thread_local std::mt19937 rng(std::random_device{}());

		uint32_t idle_spins = 0;
		static constexpr uint32_t MAX_IDLE_SPINS = 32;

		while (m_Running.load(std::memory_order_relaxed))
		{
			SJob* job = m_Dequeue.Pop();

			if (m_JobSystem)
			{
				if (!job)
				{
					job = m_JobSystem->StealFromSubmitQueue();
				}

				if (!job)
				{
					job = m_JobSystem->StealFromBusiest(m_Id);
				}

				if (!job)
				{
					job = m_JobSystem->StealFromRandom(m_Id);
				}
			}

			if (job)
			{
				idle_spins = 0; // Reset idle counter on work.
				ExecuteJob(job);
			}
			else
			{
				++idle_spins;

				// Pause for some time before yielding to reduce contention and CPU usage.
				if (idle_spins < MAX_IDLE_SPINS)
				{
#if defined (_MSC_VER)
					_mm_pause();
#elif defined(__x86_64__) || defined(__i386__)
					__builtin_ia32_pause(); // x86 pause instruction
#elif defined(__aarch64__) || defined(__arm__)
					__asm__ __volatile__("yield"); // ARM yield instruction
#endif
				}
				else
				{
					std::this_thread::yield();
					idle_spins = 0; // Reset after yielding.
				}
			}
		}

		// Drain remaining jobs before thread exits.
		while (SJob* remaining = m_Dequeue.Pop())
		{
			ExecuteJob(remaining);
		}
	}

	void CJobWorker::ExecuteJob(SJob* _job)
	{
		if (!_job || !_job->FastFunction)
		{
			return; // Invalid job, skip.
		}

		_job->FastFunction.Invoke<void>(_job);

		if (_job->Counter)
		{
			(*_job->Counter) -= 1;
		}
	}
}