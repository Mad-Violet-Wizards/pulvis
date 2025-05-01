#pragma once

namespace engine::threads
{
	//////////////////////////////////////////////////////////////////////////
	template<typename R>
	class CThreadTaskResult
	{
	public:

		void Set(R&& _result)
		{
			m_Result = std::move(_result);
		}

		R&& Get()
		{
			return m_Result;
		}

	private:

		R m_Result;
	};

	template<>
	class CThreadTaskResult<void>
	{
	public:

		void Set()
		{
			// No-op
		}

		void Get()
		{
			// No-op
		}
	};
}