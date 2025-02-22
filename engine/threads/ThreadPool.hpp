#pragma once

#include <memory>
#include <vector>

#include "Utils.hpp"
#include "ThreadPoolSettings.hpp"
#include "ThreadWorker.hpp"
#include "ThreadTask.hpp"

namespace engine::threads
{
	class CThreadPool
	{
		public:

			CThreadPool();
			explicit CThreadPool(const SThreadPoolSettings& _settings);
			~CThreadPool();

			CThreadPool(const CThreadPool&) = delete;
			CThreadPool& operator=(const CThreadPool&) = delete;

			CThreadPool(CThreadPool&& _other) noexcept;
			CThreadPool& operator=(CThreadPool&& _other) noexcept;

			void EnqueueTask(CThreadTask* task);
			void Stop();

			inline CThreadWorker& GetWorker();

		private:

			std::vector<std::unique_ptr<CThreadWorker>> m_Workers;
			std::atomic<concurrency_t> m_NextWorker;
	};
}