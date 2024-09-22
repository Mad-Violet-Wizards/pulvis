#pragma once

namespace engine::threads
{
	template<typename Ret_Val, typename... Args>
	class CThreadFunctionWrapper
	{
	public:

		CThreadFunctionWrapper() : m_FunctionPtr(nullptr) {}
		~CThreadFunctionWrapper() = default;

		template<typename Func>
		CThreadFunctionWrapper(Func&& func)
		{
			m_FunctionPtr = reinterpret_cast<function_ptr_t>(func);
		}

		CThreadFunctionWrapper(const CThreadFunctionWrapper& other) = delete;
		CThreadFunctionWrapper& operator=(const CThreadFunctionWrapper& other) = delete;

		CThreadFunctionWrapper(CThreadFunctionWrapper&& other) noexcept
		{
			m_FunctionPtr = other.m_FunctionPtr;
			other.m_FunctionPtr = nullptr;
		}

		CThreadFunctionWrapper& operator=(CThreadFunctionWrapper&& other) noexcept
		{
			if (this != &other)
			{
				m_FunctionPtr = other.m_FunctionPtr;
				other.m_FunctionPtr = nullptr;
			}

			return *this;
		}

		Ret_Val operator()(Args&&... args)
		{
			return m_FunctionPtr(std::forward<Args>(args)...);
		}

	private:

		using function_ptr_t = Ret_Val(*)(Args...);
		function_ptr_t m_FunctionPtr;
	};
}