#pragma once

#include <cstddef>
#include <type_traits>
#include <memory>
#include <iostream>

namespace pulvis::tl
{
	class FastFunction
	{
		public:

			FastFunction() = default;
			~FastFunction();

			FastFunction(FastFunction&& _other) noexcept;
			FastFunction& operator=(FastFunction&& _other) noexcept;

			FastFunction(const FastFunction&) = delete;
			FastFunction& operator=(const FastFunction&) = delete;

			template<typename R, typename... Args, typename Func>
			FastFunction(Func&& _func)
			{
				Init<R, Args...>(std::forward<Func>(_func));
			}

			template<typename R, class C, typename... Args>
			FastFunction(C* _instance, R(C::* _method)(Args...))
			{
				struct Wrapper
				{
					C* Instance;
					R(C::* Method)(Args...);


					R operator()(Args... _args)
					{
						return (Instance->*Method)(std::forward<Args>(_args)...);
					}
				};

				Init<R, Args...>(Wrapper{ _instance, _method });
			}

			template<typename R, class C, typename... Args>
			FastFunction(C* _instance, R(C::* _method)(Args...) const)
			{
				struct Wrapper
				{
					C* Instance;
					R(C::* Method)(Args...) const;


					R operator()(Args... _args)
					{
						return (Instance->*Method)(std::forward<Args>(_args)...);
					}
				};

				Init<R, Args...>(Wrapper{ _instance, _method });
			}

			template<typename R, class C, typename... Args>
			FastFunction(R(C::* _method)(Args...))
			{
				struct Wrapper
				{
					R(C::* Method)(Args...);

					R operator()(C* _instance, Args... _args)
					{
						return (_instance->*Method)(std::forward<Args>(_args)...);
					}
				};

				Init<R, C*, Args...>(Wrapper{ _method });
			}

			template<typename R, class C, typename... Args>
			FastFunction(R(C::* _method)(Args...) const)
			{
				struct Wrapper
				{
					R(C::* Method)(Args...) const;

					R operator()(const C* _instance, Args... _args)
					{
						return (_instance->*Method)(std::forward<Args>(_args)...);
					}
				};

				Init<R, const C*, Args...>(Wrapper{ _method });
			}

			template<typename R, typename... Args>
			R Invoke(Args&&... _args)
			{
				if (!m_Invoke)
					throw std::runtime_error("Fast function: Buffer empty.");


				using TypedInvokeFn = R(*)(void*, Args&&...);
				auto typed_invoke = reinterpret_cast<TypedInvokeFn>(m_Invoke);
				return typed_invoke(m_Buffer, std::forward<Args>(_args)...);
			}

			template<class R, class... Args, class Func>
			static FastFunction Make(Func&& func)
			{
				FastFunction f;
				f.Init<R, Args...>(std::forward<Func>(func));
				return f;
			}


			void Reset();

			template<typename R, typename... Args>
			R operator()(Args&&... args)
			{
				return m_Invoke(&m_Buffer, std::forward<Args>(args)...);
			}

			template<typename R, typename... Args>
			R operator()(void* _instance, Args&&... _args)
			{
				return m_Invoke(&m_Buffer, _instance, std::forward<Args>(_args)...);
			}

		private:

			void MoveFrom(FastFunction&& _other);

			template<typename R, typename... Args, typename Func>
			void Init(Func&& _func)
			{
				using F = std::decay_t<Func>;
				static_assert(sizeof(F) <= BufferSize, "Fast Function: Callable too big.");

				new (m_Buffer) F(std::forward<Func>(_func));

				using TypedInvokeFn = R(*)(void*, Args&&...);

				TypedInvokeFn typed_invoke = +[](void* buffer, Args&&... args) -> R
				{
					F& fn = *reinterpret_cast<F*>(buffer);
					return fn(std::forward<Args>(args)...);
				};

				m_Invoke = reinterpret_cast<VoidInvokeFn>(typed_invoke);

				m_Destroy = +[](void* _buffer)
				{
						reinterpret_cast<F*>(_buffer)->~F();
				};

				m_Move = +[](void* _dst, void* _src)
				{
						F& from = *reinterpret_cast<F*>(_src);
						new (_dst) F(std::move(from));
						from.~F();
				};
			}

		private:


			using VoidInvokeFn = void(*)(void);
			using DestroyFn = void(*)(void* storage);
			using MoveFn = void(*)(void* dst, void* src);

			static constexpr std::size_t BufferSize = 128;
			alignas(std::max_align_t) std::byte m_Buffer[BufferSize]{};
			VoidInvokeFn m_Invoke = nullptr;
			DestroyFn m_Destroy = nullptr;
			MoveFn m_Move = nullptr;

	};
}