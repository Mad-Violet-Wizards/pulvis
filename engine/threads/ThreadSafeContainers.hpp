#pragma once

#include <condition_variable>
#include <mutex>

namespace engine::threads
{
	template<typename T>
	class CThreadSafeQueue
	{
	public:

		explicit CThreadSafeQueue(concurrency_t _queue_size)
			: m_MaxQueueSize(_queue_size)
			, m_CurrentQueueSize(0)
			, m_QueueOpen(true)
		{

		}

		CThreadSafeQueue(const CThreadSafeQueue& _other) = delete;
		CThreadSafeQueue& operator=(const CThreadSafeQueue& _other) = delete;

		CThreadSafeQueue(CThreadSafeQueue&& _other) noexcept
			: m_MaxQueueSize(_other.m_MaxQueueSize)
			, m_CurrentQueueSize(_other.m_CurrentQueueSize)
			, m_QueueOpen(_other.m_QueueOpen)
			, m_List(std::move(_other.m_List))
		{

		}

		CThreadSafeQueue& operator=(CThreadSafeQueue&& _other) noexcept
		{
			if (this != &_other)
			{
				m_MaxQueueSize = _other.m_MaxQueueSize;
				m_CurrentQueueSize = _other.m_CurrentQueueSize;
				m_QueueOpen = _other.m_QueueOpen;
				m_List = std::move(_other.m_List);
			}
			return *this;
		}

		~CThreadSafeQueue()
		{
			Close();
		}

		bool Push(const T& _val)
		{
			std::list<T> tmp_node;
			tmp_node.push_back(_val);
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				while (m_CurrentQueueSize >= m_MaxQueueSize && m_QueueOpen)
				{
					m_cv_Push.wait(lock);
				}

				if (!m_QueueOpen)
				{
					return false;
				}

				m_List.splice(m_List.end(), tmp_node);
				m_CurrentQueueSize++;

				m_cv_Pop.notify_one();
				return true;
			}
		}

		bool Push(T&& _val)
		{
			std::list<T> tmp_node;
			tmp_node.push_back(std::move(_val));
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				while (m_CurrentQueueSize >= m_MaxQueueSize && m_QueueOpen)
				{
					m_cv_Push.wait(lock);
				}

				if (!m_QueueOpen)
				{
					return false;
				}

				m_List.splice(m_List.end(), tmp_node);
				m_CurrentQueueSize++;

				m_cv_Pop.notify_one();

				return true;
			}
		}

		bool Pop(T& _val)
		{
			std::list<T> tmp_node;

			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				while (m_List.empty() && m_QueueOpen)
				{
					m_cv_Pop.wait(lock);
				}

				if (m_List.empty() && !m_QueueOpen)
				{
					return false;
				}

				tmp_node.splice(tmp_node.end(), m_List, m_List.begin());
				m_CurrentQueueSize--;

				m_cv_Push.notify_one();
			}

			_val = tmp_node.front();
			return true;
		}

		void Close()
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_QueueOpen = false;
			m_cv_Pop.notify_all();
			m_cv_Push.notify_all();
		}
		
		bool Empty() const
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			return m_List.empty();
		}

	private:

		concurrency_t m_MaxQueueSize;
		concurrency_t m_CurrentQueueSize;
		bool m_QueueOpen;
		std::list<T> m_List;
		std::condition_variable m_cv_Push;
		std::condition_variable m_cv_Pop;
		mutable std::mutex m_Mutex;

	};
}
