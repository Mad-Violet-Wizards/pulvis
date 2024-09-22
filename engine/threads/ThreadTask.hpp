#pragma once

#include "ThreadFunctionWrapper.hpp"
#include <future>

namespace engine::threads::detail
{
	class IThreadTask
	{
		public:

			virtual ~IThreadTask() = default;
			virtual void Execute() = 0;
	};

	template<typename Ret_Val>
	class IThreadTaskReturn : public IThreadTask
	{
	public:

		virtual std::future<Ret_Val> GetFuture() = 0;
	};

	template<typename Func, typename Ret_Val = void, typename... Args>
	class CTaskImpl : public IThreadTaskReturn<Ret_Val>
	{
	public:

		CTaskImpl(Func&& _func, Args&&... _args)
			: m_Function(std::forward<Func>(_func))
			, m_Args(std::forward<Args>(_args)...)
			, m_Promise()
		{
		}

		void Execute() override final
		{
			m_Promise.set_value(ExecuteImpl(std::index_sequence_for<Args...>()));
		}

		std::future<Ret_Val> GetFuture() override final
		{
			return m_Promise.get_future();
		}

	private:

		template<size_t... Is>
		Ret_Val ExecuteImpl(std::index_sequence<Is...>)
		{
			return m_Function(std::get<Is>(m_Args)...);
		}

		CThreadFunctionWrapper<Ret_Val, Args...> m_Function;
		std::tuple<Args...> m_Args;
		std::promise<std::decay_t<Ret_Val>> m_Promise;
	};
}

namespace engine::threads
{
	class IThreadTaskImpl;

	class CThreadTask
	{
	public:

		CThreadTask() : m_Impl(nullptr) {}

		CThreadTask(CThreadTask&& _other) noexcept
			: m_Impl(std::move(_other.m_Impl))
		{
			_other.m_Impl.reset();
		}

		CThreadTask& operator=(CThreadTask&& _other) noexcept
		{
			if (this != &_other)
			{
				m_Impl = std::move(_other.m_Impl);
				_other.m_Impl.reset();
			}

			return *this;
		}

		template<typename Func, typename... Args>
		CThreadTask(Func&& _func, Args&&... _args)
		{
			using Ret_Val = std::decay_t<decltype(std::declval<Func>())>;
			m_Impl = std::make_unique<detail::CTaskImpl<Func, Ret_Val, Args...>>
			(
				std::forward<Func>(_func),
				std::forward<Args>(_args)...
			);
		}

		template<typename Ret_Val>
		std::future<Ret_Val> GetFuture()
		{
			return static_cast<detail::IThreadTaskReturn<Ret_Val>*>(m_Impl.get())->GetFuture();
		}

		void Execute()
		{
			m_Impl->Execute();
		}

		private:

			std::unique_ptr<detail::IThreadTask> m_Impl;
	};
} 