#pragma once

namespace engine::pstd
{
	template<typename R, typename... Args>
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

		R operator()(void* _instance, Args&&... args)
		{
			return m_InvokerNoInstance(&m_StorageBuffer, _instance, std::forward<Args>(args)...);
		}

	private:

		void Destroy()
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
	};
}