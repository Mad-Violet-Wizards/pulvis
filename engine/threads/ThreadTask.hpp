#pragma once

#include <functional>
#include <type_traits>

#include "ThreadTaskHandle.hpp"
#include "ThreadTaskState.hpp"
#include "engine/pstd/FastFunction.hpp"

/* engine::threads::detail */
namespace engine::threads::detail
{
//////////////////////////////////////////////////////////////////////////
	class IThreadTask
	{
		public:

			IThreadTask(std::shared_ptr<SThreadTaskState> _state)
				: m_State(std::move(_state))
			{
			}

			virtual ~IThreadTask() = default;
			virtual void Execute() = 0;

			virtual std::shared_ptr<SThreadTaskState> GetState() const
			{
				return m_State;
			}
		
		protected:

			std::shared_ptr<SThreadTaskState> m_State;
	};
	//////////////////////////////////////////////////////////////////////////
	template<typename R>
	class IThreadTaskWithResult : public IThreadTask
	{
	public:

		IThreadTaskWithResult(std::shared_ptr<SThreadTaskState> _state,
			std::shared_ptr<CThreadTaskHandle<R>> _handle)
			: IThreadTask(std::move(_state))
			, m_Handle(std::move(_handle))
		{
		}

		virtual ~IThreadTaskWithResult() = default;

		virtual std::shared_ptr<CThreadTaskHandle<R>> GetHandle() const
		{
			return m_Handle;
		}

	protected:

		std::shared_ptr<CThreadTaskHandle<R>> m_Handle;
	};

	//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
	template<typename R, typename C, typename... Args>
	class CThreadTaskClassMethodImpl : public IThreadTaskWithResult<R>
	{
		public:

			CThreadTaskClassMethodImpl(std::shared_ptr<SThreadTaskState> _state, std::shared_ptr<CThreadTaskHandle<R>> _handle, C* _instance, R(C::* _method)(Args...), Args&&... _args)
				: m_Function(_instance, _method)
				, m_Args(std::forward<Args>(_args)...)
				, IThreadTaskWithResult<R>(std::move(_state), std::move(_handle))
			{
			}

			void Execute() override final
			{
				if constexpr (std::is_void_v<R>)
				{
					ExecuteImpl(std::index_sequence_for<Args...>());
					IThreadTaskWithResult<R>::GetHandle()->m_Promise.set_value();
				}
				else
				{
					IThreadTaskWithResult<R>::GetHandle()->m_Promise.set_value(ExecuteImpl(std::index_sequence_for<Args...>()));
				}

				IThreadTask::GetState()->m_Completed.store(true);
			}

		private:

		template<size_t... Is>
		R ExecuteImpl(std::index_sequence<Is...>)
		{
			if constexpr (std::is_void_v<R>)
			{
				m_Function(std::forward<Args>(std::get<Is>(m_Args))...);
			}
			else
			{
				return m_Function(std::forward<Args>(std::get<Is>(m_Args))...);
			}
		}

		private:

			pstd::FastFunction<R, Args...> m_Function;
			std::tuple<Args...> m_Args;
	};

//////////////////////////////////////////////////////////////////////////
	template<typename R, typename Func, typename... Args>
	class CThreadTaskFunctionImpl : public IThreadTaskWithResult<R>
	{
	public:

		CThreadTaskFunctionImpl(std::shared_ptr<SThreadTaskState> _state, std::shared_ptr<CThreadTaskHandle<R>> _handle, Func&& _func, Args&&... _args)
			: m_Function(std::forward<Func>(_func))
			, m_Args(std::forward_as_tuple(_args...))
			, IThreadTaskWithResult<R>(std::move(_state), std::move(_handle))
		{
		}

		void Execute() override final
		{
			if constexpr (std::is_void_v<R>)
			{
				ExecuteImpl(std::index_sequence_for<Args...>());
				IThreadTaskWithResult<R>::GetHandle()->m_Promise.set_value();
			}
			else
			{
				IThreadTaskWithResult<R>::GetHandle()->m_Promise.set_value(ExecuteImpl(std::index_sequence_for<Args...>()));
			}

			IThreadTask::GetState()->m_Completed.store(true);
		}

	private:

		template<size_t... Is>
		R ExecuteImpl(std::index_sequence<Is...>)
		{
			if constexpr (std::is_void_v<R>)
			{
				m_Function(std::forward<Args>(std::get<Is>(m_Args))...);
			}
			else
			{
				return m_Function(std::forward<Args>(std::get<Is>(m_Args))...);
			}
		}

	private:

		pstd::FastFunction<R, Args...> m_Function;
		std::tuple<Args...> m_Args;
	};
//////////////////////////////////////////////////////////////////////////
} /* engine::threads::detail */

namespace engine::threads
{
	class CThreadTask
	{
	public:

		~CThreadTask() { delete m_Impl; }

		// Function pointer
		template <typename R, typename... Args>
		CThreadTask(const char* _task_name, R(*_func)(Args...), Args&&... _args)
		{
			using Func = R(*)(Args...);

			std::shared_ptr<SThreadTaskState> state = std::make_shared<SThreadTaskState>(_task_name);
			std::shared_ptr<CThreadTaskHandle<R>> handle = std::make_shared<CThreadTaskHandle<R>>(state);

			m_Impl = new detail::CThreadTaskFunctionImpl<R, Func, Args...>
			(
				std::move(state),
				std::move(handle),
				std::forward<Func>(_func),
				std::forward<Args>(_args)...
			);
		}

		// Class method
		template <typename R, typename C, typename... Args>
		CThreadTask(const char* _task_name, C* _instance, R(C::* _method)(Args...), Args&&... _args)
		{
			using Func = R(C::*)(Args...);

			std::shared_ptr<SThreadTaskState> state = std::make_shared<SThreadTaskState>(_task_name);
			std::shared_ptr<CThreadTaskHandle<R>> handle = std::make_shared<CThreadTaskHandle<R>>(state);

			m_Impl = new detail::CThreadTaskClassMethodImpl<R, C, Args...>
				(
					std::move(state),
					std::move(handle),
					_instance,
					std::forward<Func>(_method),
					std::forward<Args>(_args)...
				);
		}

		// Function object or lambda
		template <typename Func, typename... Args>
			CThreadTask(const char* _task_name, Func&& _func, Args&&... _args)
		{
			using R = std::decay_t<decltype(_func(std::declval<Args>()...))>;

			std::shared_ptr<SThreadTaskState> state = std::make_shared<SThreadTaskState>(_task_name);
			std::shared_ptr<CThreadTaskHandle<R>> handle = std::make_shared<CThreadTaskHandle<R>>(state);

			m_Impl = new detail::CThreadTaskFunctionImpl<R, Func, Args...>
			(
				std::move(state),
				std::move(handle),
				std::forward<Func>(_func),
				std::forward<Args>(_args)...
			);
		}

		CThreadTask(CThreadTask&& _other) noexcept
			: m_Impl(_other.m_Impl)
		{
			_other.m_Impl = nullptr;
		}

		CThreadTask& operator=(CThreadTask&& _other) noexcept
		{
			if (this != &_other)
			{
				m_Impl = _other.m_Impl;
				_other.m_Impl = nullptr;
			}

			return *this;
		}

		void Execute()
		{
			m_Impl->Execute();
		}

		std::shared_ptr<SThreadTaskState> GetState() const
		{
			return m_Impl->GetState();
		}

		template<typename R>
		std::shared_ptr<CThreadTaskHandle<R>> GetHandle() const
		{
			if (auto task = dynamic_cast<detail::IThreadTaskWithResult<R>*>(m_Impl))
			{
				return task->GetHandle();
			}
			return nullptr;
		}

		private:

			detail::IThreadTask* m_Impl{ nullptr };
	};
} 