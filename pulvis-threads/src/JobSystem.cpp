#include "JobSystem.hpp"

#include <random>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace pulvis::threads
{
	CJobSystem::CJobSystem()
		: m_SubmitQueue(DEFAULT_DEQUEUE_CAPACITY)
	{
	}

	CJobSystem::~CJobSystem()
	{
		if (IsInitialized())
		{
			Shutdown();
		}
	}

	void CJobSystem::Initialize(const SJobSystemSettings& _settings)
	{
		if (IsInitialized())
		{
			return;
		}

		const thread_id_t thread_count = std::min(_settings.ThreadsCount, std::thread::hardware_concurrency());

		m_Workers.reserve(thread_count);

		for (thread_id_t i = 0; i < thread_count; ++i)
		{
			m_Workers.emplace_back(std::make_unique<CJobWorker>());
		}

		for (thread_id_t i = 0; i < thread_count; ++i)
		{
			m_Workers[i]->Start(this, i);
		}

		m_Initialized = true;
	}

	void CJobSystem::Shutdown()
	{
		if (!m_Initialized)
		{
			return;
		}

		for (auto& worker : m_Workers)
		{
			worker->RequestStop();
		}

		for (auto& worker : m_Workers)
		{
			worker->Join();
		}

		m_Workers.clear();
		m_Initialized = false;
	}

	void CJobSystem::Submit(SJob* _job)
	{
		m_SubmitQueue.Push(_job);
	}

	void CJobSystem::SubmitBatch(SJob** _jobs, size_t _count)
	{
		for (size_t i = 0; i < _count; ++i)
		{
			m_SubmitQueue.Push(_jobs[i]);
		}
	}

	void CJobSystem::Wait(CJobCounter* _counter)
	{
		uint32_t idle_spins = 0;
		static constexpr uint32_t MAX_IDLE_SPINS = 64;

		while (!_counter->IsReady())
		{
			if (HelpWithWork())
			{
				idle_spins = 0; // Reset idle spins if we were able to help with work.
			}
			else
			{
				++idle_spins;

				if (idle_spins >= MAX_IDLE_SPINS)
				{
					std::this_thread::yield(); // Yield to other threads after spinning for a while.
					idle_spins = 0;
				}
				else
				{
#if defined(_MSC_VER)
					_mm_pause(); // Pause instruction to reduce contention on hyper-threaded cores.
#elif defined(__x86_64__) || defined(__i386__)
					__builtin_ia32_pause(); // GCC/Clang intrinsic for pause on x86.
#elif defined(__aarch64__) || defined(__arm__)
					__asm__ __volatile__("yield"); // ARM yield instruction
#endif
				}
				// Back off with _mm_pause to reduce contention on hyper-threaded cores.
			}
		}
	}

	bool CJobSystem::HelpWithWork()
	{
		SJob* job = m_SubmitQueue.Pop();

		if (!job)
		{
			for (auto& worker : m_Workers)
			{
				job = worker->StealJob();
				if (job)
				{
					break;
				}
			}
		}

		if (job)
		{
			if (job->FastFunction)
			{
				job->FastFunction.Invoke<void>(job);
			}

			if (job->Counter != nullptr)
			{
				job->Counter->Decrement();
			}

			return true;
		}

		return false;
	}

	SJob* CJobSystem::StealFromSubmitQueue()
	{
		return m_SubmitQueue.Steal();
	}

	SJob* CJobSystem::StealFromRandom(thread_id_t _thiefId)
	{
		if (m_Workers.size() <= 1)
		{
			return nullptr;
		}

		thread_local std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<thread_id_t> dist(0, static_cast<thread_id_t>(m_Workers.size() - 1));

		static constexpr int MAX_STEAL_ATTEMPTS = 3;

		for (int attempt = 0; attempt < MAX_STEAL_ATTEMPTS; ++attempt)
		{
			const thread_id_t victim = dist(rng);

			if (victim == _thiefId)
			{
				continue;
			}

			SJob* stolen = m_Workers[victim]->StealJob();
			if (stolen != nullptr)
			{
				return stolen;
			}
		}

		return nullptr;
	}

	SJob* CJobSystem::StealFromBusiest(thread_id_t _thiefId)
	{
		if (m_Workers.size() <= 1)
		{
			return nullptr;
		}

		thread_id_t busiest = _thiefId;
		int64_t max_queue_size = -1;

		for (thread_id_t i = 0; i < static_cast<thread_id_t>(m_Workers.size()); ++i)
		{
			if (i == _thiefId)
			{
				continue;
			}

			int64_t queue_size = m_Workers[i]->GetQueueSize();
			if (queue_size > max_queue_size)
			{
				max_queue_size = queue_size;
				busiest = i;
			}
		}

		if (busiest != _thiefId)
		{
			return m_Workers[busiest]->StealJob();
		}

		return nullptr;
	}

	void CJobSystem::ResetAllocators()
	{
		for (auto& worker : m_Workers)
		{
			worker->ResetAllocator();
		}
	}

	thread_id_t CJobSystem::GetNextWorkerIndex()
	{
		return m_NextWorkerIndex.fetch_add(1, std::memory_order_relaxed) % static_cast<thread_id_t>(m_Workers.size());
	}

}