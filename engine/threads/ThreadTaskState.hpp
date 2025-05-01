#pragma once

#include "engine/pstd/StringUtils.hpp"

namespace engine::threads
{
	class CThreadTask;

	struct SThreadTaskState
	{
		SThreadTaskState(const char* _task_name)
			: m_TaskID(pstd::ConstexprStringHash(_task_name)) {}

		SThreadTaskState(const SThreadTaskState&) = delete;
		SThreadTaskState& operator=(const SThreadTaskState&) = delete;

		SThreadTaskState(SThreadTaskState&&) = default;
		SThreadTaskState& operator=(SThreadTaskState&&) = default;
		
		uint32_t m_TaskID{ 0 };
		std::atomic<bool> m_Cancelled{ false };
		std::atomic<bool> m_Completed{ false };
		std::atomic<bool> m_Delayed{ false };
		std::atomic<bool> m_Started{ false };
	};
}
