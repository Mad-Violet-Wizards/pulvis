#include "engine/engine_pch.hpp"
#include "ThreadWorker.hpp"

namespace engine::threads
{
	CThreadWorker::CThreadWorker(concurrency_t _queue_size)
		: m_TaskQueue(_queue_size)
		, m_ThreadID(-1)
	{
	}

	CThreadWorker::~CThreadWorker()
	{
		if (m_Running.load())
		{
			Stop();
		}
	}

	CThreadWorker::CThreadWorker(CThreadWorker&& _other) noexcept
		: m_TaskQueue(std::move(_other.m_TaskQueue))
		, m_ThreadID(_other.m_ThreadID)
		, m_Running(_other.m_Running.load())
		, m_Thread(std::move(_other.m_Thread))
	{

	}

	CThreadWorker& CThreadWorker::operator=(CThreadWorker&& _other) noexcept
	{
		if (this != &_other)
		{
			m_TaskQueue = std::move(_other.m_TaskQueue);
			m_ThreadID = _other.m_ThreadID;
			m_Running = _other.m_Running.load();
			m_Thread = std::move(_other.m_Thread);
		}
		return *this;
	}

	void CThreadWorker::Start(concurrency_t _thread_id)
	{
		m_ThreadID = _thread_id;
		m_Running.store(true);
		m_Thread = std::thread(&CThreadWorker::Run, this);
	}

	void CThreadWorker::Run()
	{
		while (m_Running.load())
		{
			CThreadTask* task;
			if (m_TaskQueue.Pop(task))
			{
				task->Execute();
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	}

	void CThreadWorker::Stop()
	{
		m_Running.store(false);
		m_TaskQueue.Close();
	}

	void CThreadWorker::JoinThread()
	{
		if (m_Thread.joinable())
		{
			m_Thread.join();
		}
	}

	void CThreadWorker::EnqueueTask(CThreadTask* _task)
	{
		m_TaskQueue.Push(_task);
	}
}
