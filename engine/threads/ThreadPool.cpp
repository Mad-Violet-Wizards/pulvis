#include "engine/engine_pch.hpp"
#include "ThreadPool.hpp"

namespace engine::threads
{
	CThreadPool::CThreadPool(const SThreadPoolSettings& _settings)
		: m_NextWorker(0)
	{
		m_Workers.reserve(_settings.m_NumThreads);

		for (concurrency_t idx = 0; idx < _settings.m_NumThreads; ++idx)
		{
			m_Workers.emplace_back(std::make_unique<CThreadWorker>(_settings.m_QueueSize));
		}

		for (concurrency_t idx = 0; idx < _settings.m_NumThreads; ++idx)
		{
			m_Workers[idx]->Start(idx);
		}
	}

	CThreadPool::~CThreadPool()
	{
		Stop();
	}

	void CThreadPool::EnqueueTask(CThreadTask* task)
	{
		GetWorker().EnqueueTask(task);
	}

	CThreadPool::CThreadPool(CThreadPool&& _other) noexcept
		: m_Workers(std::move(_other.m_Workers))
	{
		_other.m_Workers.clear();
	}

	CThreadPool& CThreadPool::operator=(CThreadPool&& _other) noexcept
	{
		if (this != &_other)
		{
			m_Workers = std::move(_other.m_Workers);
			_other.m_Workers.clear();
		}

		return *this;
	}

	void CThreadPool::Stop()
	{
		for (auto& worker : m_Workers)
		{
			if (worker)
			{
				worker->Stop();
			}
		}

		for (auto& worker : m_Workers)
		{
			if (worker)
			{
				worker->JoinThread();
			}
		}
	}

	inline CThreadWorker& CThreadPool::GetWorker()
	{
		const concurrency_t idx = m_NextWorker.fetch_add(1) % m_Workers.size();
		return *m_Workers[idx];
	}
}