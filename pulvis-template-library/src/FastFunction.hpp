		#pragma once

		#include <cstddef>
		#include <type_traits>
		#include <memory>
		#include <stdexcept>

		namespace pulvis::tl
		{
			/* 
				Type-erased, non-owning, move-only callable wrapper with inline storage.
				Avoids heap allocation by storing callables in a fixed-size inline buffer (SBO).

				Design goals:
				 - Zero heap allocation for callables up to 24 bytes.
				 - No RTTI, no exceptions, no std::function overhead.
				 - Supports free functions, lambdas, member function pointers (bound and unbound).

				Caller must specify return type and argument types explicitly via template parameters.

				Buffer size: 24 bytes (fits up to 3 pointers worth of captures).
				Total size:  48 bytes (buffer + invoke/destroy/move function pointers).

				For callables exceeding 24 bytes of capture, pass a pointer to externally managed data.

				Typical usage:
					// Lambda
					auto fn = FastFunction::Make<void, int>([&obj](int x) { obj.Process(x); });
					fn.Invoke<void, int>(42);

					// Bound member function
					FastFunction fn(&instance, &MyClass::DoWork);
					fn.Invoke<void>(args...);

					// Unbound member function (first arg is instance pointer)
					FastFunction fn(&MyClass::DoWork);
					fn.Invoke<void, MyClass*>(&instance, args...);

					// Check if callable is set
					if (fn) { fn.Invoke<void>(); }

					// Reset
					fn.Reset();
			*/

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
						using TypedInvokeFn = R(*)(void*, Args...);
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

					explicit operator bool() const noexcept { return m_Invoke != nullptr; }

					template<typename R, typename... Args>
					R operator()(Args&&... args)
					{
						using TypedInvokeFn = R(*)(void*, Args...);
						auto typed_invoke = reinterpret_cast<TypedInvokeFn>(m_Invoke);
						return typed_invoke(m_Buffer, std::forward<Args>(args)...);
					}

				private:

					void MoveFrom(FastFunction&& _other);

					template<typename R, typename... Args, typename Func>
					void Init(Func&& _func)
					{
						using F = std::decay_t<Func>;
						static_assert(sizeof(F) <= BufferSize, "Fast Function: Callable too big.");
						static_assert(alignof(F) <= alignof(std::max_align_t), "Fast Function: Callable alignment too strict.");

						new (m_Buffer) F(std::forward<Func>(_func));

						m_Invoke = reinterpret_cast<VoidInvokeFn>(
							+[](void* buffer, Args... args) -> R
							{
								return (*static_cast<F*>(buffer))(std::forward<Args>(args)...);
							}
							);

						m_Destroy = +[](void* _buffer)
							{
								static_cast<F*>(_buffer)->~F();
							};

						m_Move = +[](void* _dst, void* _src)
							{
								F& from = *static_cast<F*>(_src);
								new (_dst) F(std::move(from));
								from.~F();
							};
					}

				private:

					using VoidInvokeFn = void(*)(void);
					using DestroyFn = void(*)(void* storage);
					using MoveFn = void(*)(void* dst, void* src);

					static constexpr std::size_t BufferSize = 24;
					alignas(std::max_align_t) std::byte m_Buffer[BufferSize];
					VoidInvokeFn m_Invoke = nullptr;
					DestroyFn m_Destroy = nullptr;
					MoveFn m_Move = nullptr;
			};
		}