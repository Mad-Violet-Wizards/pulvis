#pragma once

#include "Utils.hpp"
#include "ThreadTask.hpp"
#include "ThreadSafeContainers.hpp"

namespace engine::threads
{
	class CThreadWorker
	{
		public:

			explicit CThreadWorker(concurrency_t _queue_size);
			~CThreadWorker();

			CThreadWorker(CThreadWorker&& _other) noexcept;
			CThreadWorker& operator=(CThreadWorker&& _other) noexcept;

			std::atomic<bool>& IsRunning() { return m_Running; }

			void Start(concurrency_t _thread_id);
			void Stop();
			void JoinThread();

			void EnqueueTask(CThreadTask* _task);

			void Run();

		private:
			
			CThreadSafeQueue<CThreadTask*> m_TaskQueue;
			concurrency_t m_ThreadID;
			std::atomic<bool> m_Running;
			std::thread m_Thread;

	};
}