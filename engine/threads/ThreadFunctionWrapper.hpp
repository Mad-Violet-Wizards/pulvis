#pragma once

namespace engine::threads
{
	template<typename R, typename... Args>
	class CThreadFunctionWrapper
	{
	public:

		CThreadFunctionWrapper()
			: m_Invoker(nullptr)
			, m_Destructor(nullptr) 
		{
		}

		~CThreadFunctionWrapper()
		{
			Destroy();
		}

		using function_t = R(*)(Args...);

		template<typename Func>
		CThreadFunctionWrapper(Func&& _func)
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
		CThreadFunctionWrapper(C* _instance, R(C::* _method)(Args...))
		{
			struct SClassMethodWrapper
			{
				C* m_Instance;
				R(C::* m_Methdod)(Args...);

				SClassMethodWrapper(C* _instance, R(C::* _method)(Args...))
					: m_Instance(_instance)
					, m_Methdod(_method) {}

				R operator()(Args&&... _args)
				{
					return (m_Instance->*m_Methdod)(std::forward<Args>(_args)...);
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

		CThreadFunctionWrapper(const CThreadFunctionWrapper& _other) = delete;
		CThreadFunctionWrapper& operator=(const CThreadFunctionWrapper& _other) = delete;

		CThreadFunctionWrapper(CThreadFunctionWrapper&& _other) noexcept
		{
			std::memcpy(&m_StorageBuffer, &_other.m_StorageBuffer, sizeof(m_StorageBuffer));
			m_Invoker = _other.m_Invoker;
			m_Destructor = _other.m_Destructor;
			_other.m_Invoker = nullptr;
			_other.m_Destructor = nullptr;
		}

		CThreadFunctionWrapper& operator=(CThreadFunctionWrapper&& _other) noexcept
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
		typename std::aligned_storage<s_BufferSize, sizeof(size_t)>::type m_StorageBuffer;

		using Invoker = R(*)(void*, Args...);
		using Destructor = void(*)(void*);

		Destructor m_Destructor;
		Invoker m_Invoker;
	};
}