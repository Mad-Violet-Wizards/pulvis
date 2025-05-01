#pragma once

#include <future>

#include "ThreadTaskState.hpp"

namespace engine::threads
{
	template<typename R>
	class CThreadTaskHandle
	{
		public:
			CThreadTaskHandle(std::shared_ptr<SThreadTaskState> _state)
				: m_Future(m_Promise.get_future())
				, m_State(std::move(_state))
			{
			}

			CThreadTaskHandle(const CThreadTaskHandle&) = delete;
			CThreadTaskHandle& operator=(const CThreadTaskHandle&) = delete;

			CThreadTaskHandle(CThreadTaskHandle&& _other) noexcept
				: m_State(std::move(_other.m_State))
			{
				_other.m_State = nullptr;
			}

			CThreadTaskHandle& operator=(CThreadTaskHandle&& _other) noexcept
			{
				if (this != &_other)
				{
					m_State = std::move(_other.m_State);
					_other.m_State = nullptr;
				}
				return *this;
			}

			~CThreadTaskHandle() = default;

		public:

			bool GetIsCompleted() const
			{
				return m_State->m_Completed.load();
			}

			bool GetIsCancelled() const
			{
				return m_State->m_Cancelled.load();
			}

			bool GetIsDelayed() const
			{
				return m_State->m_Delayed.load();
			}

			bool GetIsStarted() const
			{
				return m_State->m_Started.load();
			}

			void Cancel()
			{
				m_State->m_Cancelled.store(true);
			}

			R GetResult()
			{
				return m_Future.get();
			}

			bool Wait() const
			{
				return m_Future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
			}

		public:

				std::shared_ptr<SThreadTaskState> m_State;
				std::promise<R> m_Promise;
				std::future<R> m_Future;
		};
	};