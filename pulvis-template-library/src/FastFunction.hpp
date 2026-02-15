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

				Init<R, C*, Args...>(Wrapper{ _instance, _method });
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

				Init<R, const C*, Args...>(Wrapper{ _instance, _method });
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

				std::tuple<Args...> tuple(std::forward<Args>(_args)...);
				if constexpr (std::is_void_v<R>)
				{
					m_Invoke(m_Buffer, &tuple, nullptr);
				}
				else
				{
					R out{};
					m_Invoke(m_Buffer, &tuple, &out);
					return out;
				}
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

				m_Invoke = +[](void* buffer, void* tuplePtr, void* outPtr)
					{
						F& fn = *reinterpret_cast<F*>(buffer);
						auto& tuple = *reinterpret_cast<std::tuple<Args...>*>(tuplePtr);

						if constexpr (std::is_void_v<R>)
						{
							std::apply([&](auto&&... _arg)
								{
									fn(std::forward<decltype(_arg)>(_arg)...);
								}, tuple);
						}
						else
						{
							R& out = *reinterpret_cast<R*>(outPtr);
							out = std::apply([&](auto&&... _arg) -> R
								{
									return fn(std::forward<decltype(_arg)>(_arg)...);
								}, tuple);
						}
					};

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


			using InvokeFn = void(*)(void* storage, void* tuplePtr, void* outPtr);
			using DestroyFn = void(*)(void* storage);
			using MoveFn = void(*)(void* dst, void* src);

			static constexpr std::size_t BufferSize = 128;
			alignas(std::max_align_t) std::byte m_Buffer[BufferSize]{};
			InvokeFn m_Invoke = nullptr;
			DestroyFn m_Destroy = nullptr;
			MoveFn m_Move = nullptr;

	};

	/*template<typename R, typename... Args>
	class FastFunction
	{
	public:

		FastFunction()
			: m_Invoker(nullptr)
			, m_Destructor(nullptr)
		{
		}

		~FastFunction()
		{
			Destroy();
		}

		using function_t = R(*)(Args...);

		template<typename Func>
		explicit FastFunction(Func&& _func)
		{
			using function_unref_t = std::decay_t<Func>;
			static_assert(sizeof(function_unref_t) < sizeof(m_StorageBuffer), "Function object is too big to fit in the buffer");
			new (static_cast<void*>(&m_StorageBuffer)) function_unref_t(std::forward<Func>(_func));

			m_Invoker = [](void* _buffer, Args... _args) -> R
				{
					auto& function_ref = *static_cast<function_unref_t*>(_buffer);
					static_assert(std::is_same<R, decltype(function_ref(std::forward<Args>(_args)...))>::value, "Type mismatch!");

					return function_ref(std::forward<Args>(_args)...);
				};

			m_Destructor = [](void* _buffer)
				{
					static_cast<function_unref_t*>(_buffer)->~function_unref_t();
				};
		}

		template<class C>
		explicit FastFunction(C* _instance, R(C::* _method)(Args...) const)
		{
			struct SClassMethodWrapper
			{
				C* m_Instance;
				R(C::* m_Method)(Args...) const;

				SClassMethodWrapper(C* _instance, R(C::* _method)(Args...) const)
					: m_Instance(_instance)
					, m_Method(_method) {}

				R operator()(Args&&... _args)
				{
					return (m_Instance->*m_Method)(std::forward<Args>(_args)...);
				}
			};

			static_assert(sizeof(SClassMethodWrapper) < sizeof(m_StorageBuffer), "Function object is too big to fit in the buffer");
			new (static_cast<void*>(&m_StorageBuffer)) SClassMethodWrapper(_instance, _method);

			m_Invoker = +[](void* buffer, Args... args) -> R
				{
					auto& method_ref = *static_cast<SClassMethodWrapper*>(buffer);
					return method_ref(std::forward<Args>(args)...);
				};

			m_Destructor = [](void* buffer)
				{
					static_cast<SClassMethodWrapper*>(buffer)->~SClassMethodWrapper();
				};
		}

		template<class C>
		explicit FastFunction(R(C::* _method)(Args...) const)
		{
			struct SClassMethodWrapper
			{
				R(C::* m_Method)(Args...) const;

				SClassMethodWrapper(R(C::* _method)(Args...) const)
					: m_Method(_method) {}

				R operator()(C* _instance, Args&&... _args)
				{
					return (_instance->*m_Method)(std::forward<Args>(_args)...);
				}
			};

			static_assert(sizeof(SClassMethodWrapper) < sizeof(m_StorageBuffer), "Function object is too big to fit in the buffer");
			new (static_cast<void*>(&m_StorageBuffer)) SClassMethodWrapper(_method);

			m_InvokerNoInstance = +[](void* buffer, void* instance, Args... args) -> R
				{
					auto& method_ref = *static_cast<SClassMethodWrapper*>(buffer);
					return method_ref(static_cast<C*>(instance), std::forward<Args>(args)...);
				};

			m_Destructor = [](void* buffer)
				{
					static_cast<SClassMethodWrapper*>(buffer)->~SClassMethodWrapper();
				};
		}

		template<class C>
		explicit FastFunction(C* _instance, R(C::* _method)(Args...))
		{
			struct SClassMethodWrapper
			{
				C* m_Instance;
				R(C::* m_Method)(Args...);

				SClassMethodWrapper(C* _instance, R(C::* _method)(Args...))
					: m_Instance(_instance)
					, m_Method(_method) {}

				R operator()(Args&&... _args)
				{
					return (m_Instance->*m_Method)(std::forward<Args>(_args)...);
				}
			};

			static_assert(sizeof(SClassMethodWrapper) < sizeof(m_StorageBuffer), "Function object is too big to fit in the buffer");
			new (static_cast<void*>(&m_StorageBuffer)) SClassMethodWrapper(_instance, _method);

			m_Invoker = +[](void* buffer, Args... args) -> R
				{
					auto& method_ref = *static_cast<SClassMethodWrapper*>(buffer);
					return method_ref(std::forward<Args>(args)...);
				};

			m_Destructor = [](void* buffer)
				{
					static_cast<SClassMethodWrapper*>(buffer)->~SClassMethodWrapper();
				};
		}

		template<class C>
		explicit FastFunction(R(C::* _method)(Args...))
		{
			struct SClassMethodWrapper
			{
				R(C::* m_Method)(Args...);

				SClassMethodWrapper(R(C::* _method)(Args...))
					: m_Method(_method) {}

				R operator()(C* _instance, Args&&... _args)
				{
					return (_instance->*m_Method)(std::forward<Args>(_args)...);
				}
			};

			static_assert(sizeof(SClassMethodWrapper) < sizeof(m_StorageBuffer), "Function object is too big to fit in the buffer");
			new (static_cast<void*>(&m_StorageBuffer)) SClassMethodWrapper(_method);

			m_InvokerNoInstance = +[](void* buffer, void* instance, Args... args) -> R
				{
					auto& method_ref = *static_cast<SClassMethodWrapper*>(buffer);
					return method_ref(static_cast<C*>(instance), std::forward<Args>(args)...);
 				};

			m_Destructor = [](void* buffer)
				{
					static_cast<SClassMethodWrapper*>(buffer)->~SClassMethodWrapper();
				};
		}


		FastFunction(const FastFunction& _other) = delete;
		FastFunction& operator=(const FastFunction& _other) = delete;

		FastFunction(FastFunction&& _other) noexcept
		{
			std::memcpy(&m_StorageBuffer, &_other.m_StorageBuffer, sizeof(m_StorageBuffer));
			m_Invoker = _other.m_Invoker;
			m_Destructor = _other.m_Destructor;
			_other.m_Invoker = nullptr;
			_other.m_Destructor = nullptr;
		}

		FastFunction& operator=(FastFunction&& _other) noexcept
		{
			if (this != &_other)
			{
				Destroy();
				std::memcpy(&m_StorageBuffer, &_other.m_StorageBuffer, sizeof(m_StorageBuffer));
				m_Invoker = _other.m_Invoker;
				m_Destructor = _other.m_Destructor;
				_other.m_Invoker = nullptr;
				_other.m_Destructor = nullptr;
			}

			return *this;
		}

		R operator()(Args&&... args)
		{
			return m_Invoker(&m_StorageBuffer, std::forward<Args>(args)...);
		}

		R operator()(void* _instance, Args&&... _args)
		{
			return m_InvokerNoInstance(&m_StorageBuffer, _instance, std::forward<Args>(_args)...);
		}

	private:

		void Destroy() override
		{
			if (m_Destructor)
			{
				m_Destructor(&m_StorageBuffer);
			}
		}

	private:

		static constexpr size_t s_BufferSize = 128;
		alignas(sizeof(size_t)) std::byte m_StorageBuffer[s_BufferSize];

		using Invoker = R(*)(void*, Args...);
		using InvokerNoInstance = R(*)(void*, void*, Args...);
		using Destructor = void(*)(void*);

		Destructor m_Destructor = nullptr;
		Invoker m_Invoker = nullptr;
		InvokerNoInstance m_InvokerNoInstance = nullptr;
	};*/
}