#include "engine/engine_pch.hpp"
#include "ThreadPool.hpp"

namespace engine::threads
{
	class CThreadPool::Impl
	{
		public:

			std::vector<std::unique_ptr<CThreadWorker>> m_Workers;
			std::atomic<concurrency_t> m_NextWorker;
	};

	CThreadPool::CThreadPool()
	{
		SThreadPoolSettings settings;
		settings.m_NumThreads = std::thread::hardware_concurrency();
		settings.m_QueueSize = S_DEFAULT_QUEUE_SIZE;

		m_Impl = new Impl();
		m_Impl->m_NextWorker = 0;

		m_Impl->m_Workers.reserve(settings.m_NumThreads);

		for (concurrency_t idx = 0; idx < settings.m_NumThreads; ++idx)
		{
			m_Impl->m_Workers.emplace_back(std::make_unique<CThreadWorker>(settings.m_QueueSize));
		}

		for (concurrency_t idx = 0; idx < settings.m_NumThreads; ++idx)
		{
			m_Impl->m_Workers[idx]->Start(idx);
		}
	}

	CThreadPool::CThreadPool(const SThreadPoolSettings& _settings)
	{
		m_Impl = new Impl();
		m_Impl->m_NextWorker = 0;

		m_Impl->m_Workers.reserve(_settings.m_NumThreads);

		for (concurrency_t idx = 0; idx < _settings.m_NumThreads; ++idx)
		{
			m_Impl->m_Workers.emplace_back(std::make_unique<CThreadWorker>(_settings.m_QueueSize));
		}

		for (concurrency_t idx = 0; idx < _settings.m_NumThreads; ++idx)
		{
			m_Impl->m_Workers[idx]->Start(idx);
		}
	}

	CThreadPool::~CThreadPool()
	{
		Stop();
		delete m_Impl;
	}

	void CThreadPool::EnqueueTask(CThreadTask* task)
	{
		GetWorker().EnqueueTask(task);
	}

	CThreadPool::CThreadPool(CThreadPool&& _other) noexcept
	{
		m_Impl->m_Workers = std::move(_other.m_Impl->m_Workers);
		_other.m_Impl->m_Workers.clear();
	}

	CThreadPool& CThreadPool::operator=(CThreadPool&& _other) noexcept
	{
		if (this != &_other)
		{
			m_Impl->m_Workers = std::move(_other.m_Impl->m_Workers);
			_other.m_Impl->m_Workers.clear();
		}

		return *this;
	}

	void CThreadPool::Stop()
	{
		for (auto& worker : m_Impl->m_Workers)
		{
			if (worker)
			{
				worker->Stop();
			}
		}

		for (auto& worker : m_Impl->m_Workers)
		{
			if (worker)
			{
				worker->JoinThread();
			}
		}
	}

	inline CThreadWorker& CThreadPool::GetWorker()
	{
		const concurrency_t idx = m_Impl->m_NextWorker.fetch_add(1) % m_Impl->m_Workers.size();
		return *m_Impl->m_Workers[idx];
	}
}