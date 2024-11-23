#pragma once

#include <future>
#include <functional>
#include <type_traits>

#include "engine/pstd/FastFunction.hpp"

/* engine::threads::detail */
namespace engine::threads::detail
{
//////////////////////////////////////////////////////////////////////////
	class IThreadTask
	{
		public:

			virtual ~IThreadTask() = default;
			virtual void Execute() = 0;
	};
//////////////////////////////////////////////////////////////////////////
	template<typename R>
	class IThreadTaskFuture : public IThreadTask
	{
		public:

			virtual ~IThreadTaskFuture() = default;
			virtual std::future<R> GetFuture() = 0;
	};

	template<typename R, typename C, typename... Args>
	class CThreadTaskClassMethodImpl : public IThreadTaskFuture<R>
	{
		public:

			CThreadTaskClassMethodImpl(C* _instance, R(C::* _method)(Args...), Args&&... _args)
				: m_Function(_instance, _method)
				, m_Args(std::forward<Args>(_args)...)
			{
			}

			void Execute() override final
			{
				if constexpr (std::is_void_v<R>)
				{
					ExecuteImpl(std::index_sequence_for<Args...>());
					m_Promise.set_value();
				}
				else
				{
					m_Promise.set_value(ExecuteImpl(std::index_sequence_for<Args...>()));
				}
			}

			std::future<R> GetFuture() override final
			{
				return m_Promise.get_future();
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
			std::promise<std::decay_t<R>> m_Promise;
	};

//////////////////////////////////////////////////////////////////////////
	template<typename R, typename Func, typename... Args>
	class CThreadTaskFunctionImpl : public IThreadTaskFuture<R>
	{
	public:

		CThreadTaskFunctionImpl(Func&& _func, Args&&... _args)
			: m_Function(std::forward<Func>(_func))
			, m_Args(std::forward_as_tuple(_args...))
		{
		}

		virtual void Execute() override final
		{
			if constexpr (std::is_void_v<R>)
			{
				ExecuteImpl(std::index_sequence_for<Args...>());
				m_Promise.set_value();
			}
			else
			{
				m_Promise.set_value(ExecuteImpl(std::index_sequence_for<Args...>()));
			}
		}

		std::future<R> GetFuture() override final
		{
			return m_Promise.get_future();
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
		std::promise<std::decay_t<R>> m_Promise;
	};
//////////////////////////////////////////////////////////////////////////
} /* engine::threads::detail */

namespace engine::threads
{
	class IThreadTaskImpl;

	class PULVIS_API CThreadTask
	{
	public:

		CThreadTask() : m_Impl(nullptr) {}

		// Function pointer
		template <typename R, typename... Args>
		CThreadTask(R(*_func)(Args...), Args&&... _args)
		{
			using Func = R(*)(Args...);
			m_Impl = std::make_unique<detail::CThreadTaskFunctionImpl<R, Func, Args...>>
			(
				std::forward<Func>(_func),
				std::forward<Args>(_args)...
			);
		}

		// Class method
		template <typename R, typename C, typename... Args>
		CThreadTask(C* _instance, R(C::* _method)(Args...), Args&&... _args)
		{
			using Func = R(C::*)(Args...);
			m_Impl = std::make_unique<detail::CThreadTaskClassMethodImpl<R, C, Args...>>
				(
					_instance,
					std::forward<Func>(_method),
					std::forward<Args>(_args)...
				);

		}

		// Function object or lambda
		template <typename Func, typename... Args>
			CThreadTask(Func&& _func, Args&&... _args)
		{
			using R = std::decay_t<decltype(_func(std::declval<Args>()...))>;
			m_Impl = std::make_unique<detail::CThreadTaskFunctionImpl<R, Func, Args...>>
			(
				std::forward<Func>(_func),
				std::forward<Args>(_args)...
			);
		}

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

		template<typename R>
		std::future<R> GetFuture() 
		{
			return static_cast<detail::IThreadTaskFuture<R>*>(m_Impl.get())->GetFuture();
		}

		void Execute()
		{
			m_Impl->Execute();
		}

		private:

			std::unique_ptr<detail::IThreadTask> m_Impl;
	};
} 