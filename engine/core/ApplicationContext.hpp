#pragma once

#include "engine/filesystem/Filesystem.hpp"
#include "ApplicationStateMachine.hpp"
#include "Setup.hpp"
#include "engine/threads/ThreadPool.hpp"
#include "engine/threads/ThreadTaskScheduler.hpp"

namespace engine::core
{
	class CApplicationContext
	{
		public:

			mutable engine::threads::CThreadPool m_ThreadPool;
			mutable engine::threads::CThreadTaskScheduler m_ThreadTaskScheduler{ m_ThreadPool };
			SApplicationSetup m_Setup;
			ApplicationStateMachine m_StateMachine;
			uint64_t m_FrameNumber;
			float m_DeltaTime;
			engine::fs::Filesystem* m_Filesystem;
	};
}