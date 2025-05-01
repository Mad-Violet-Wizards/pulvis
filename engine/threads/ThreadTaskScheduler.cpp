#include "engine/engine_pch.hpp"
#include "ThreadTaskScheduler.hpp"

namespace engine::threads
{
	CThreadTaskScheduler::CThreadTaskScheduler(CThreadPool& _threadPool)
		: m_ThreadPool(_threadPool)
	{
	}

	CThreadTaskScheduler::~CThreadTaskScheduler()
	{
	}

	void CThreadTaskScheduler::Process()
	{
		ProcessCancelledTasks();
		ProcessDelayedTasks();
		ProcessCompletedTasks();
		ProcessPendingTasks();
	}

	void CThreadTaskScheduler::ReleaseCompletedTask(const char* _task_name)
	{
		for (auto it = m_CompletedTasks.begin(); it != m_CompletedTasks.end(); ++it)
		{
			if ((*it)->m_Task->GetState()->m_TaskID == pstd::ConstexprStringHash(_task_name))
			{
				delete (*it)->m_Task;
				(*it)->m_Task = nullptr;
				delete (*it);
				m_CompletedTasks.erase(it);
				break;
			}
		}
	}

	void CThreadTaskScheduler::FillSingleProcessData(SThreadTaskSchedulerSingleProcessData& _single_proc_data)
	{
		// Always process high priority tasks.

		const size_t high_priority_count = m_PendingTasks[ETaskPriority::High].size();
		const size_t normal_priority_count = m_PendingTasks[ETaskPriority::Normal].size();

		constexpr static short threshold_to_disable_normal_priority_tasks = 128;
		constexpr static short threshold_to_disable_low_priority_tasks = 64;

		if (high_priority_count > threshold_to_disable_normal_priority_tasks)
		{
			_single_proc_data.m_ProcessNormalPriorityTasks = false;
		}

		if (high_priority_count + normal_priority_count > threshold_to_disable_low_priority_tasks)
		{
			_single_proc_data.m_ProcessLowPriorityTasks = false;
		}

	}

	void CThreadTaskScheduler::FillTasksToEnqueue(std::vector<SThreadTaskSchedulerEntity*>& _tasks_to_enqueue, ETaskPriority _priority)
	{
		std::vector<SThreadTaskSchedulerEntity*>& tasks = m_PendingTasks[_priority];

		for (SThreadTaskSchedulerEntity* scheduler_entity : tasks)
		{
			if (scheduler_entity->m_Task->GetState()->m_Started.load())
			{
				continue;
			}

			if (scheduler_entity->m_Task->GetState()->m_Cancelled.load())
			{
				continue;
			}

			if (scheduler_entity->m_Task->GetState()->m_Delayed.load())
			{
				continue;
			}

			if (scheduler_entity->m_Task->GetState()->m_Completed.load())
			{
				continue;
			}

			_tasks_to_enqueue.push_back(scheduler_entity);
		}
	}

	void CThreadTaskScheduler::ProcessCancelledTasks()
	{
		for (auto &[priority, tasks] : m_PendingTasks)
		{
			for (auto it = tasks.begin(); it != tasks.end();)
			{
				SThreadTaskSchedulerEntity* scheduler_entity = (*it);
				if (scheduler_entity->m_Task->GetState()->m_Cancelled.load())
				{
					it = tasks.erase(it);

					delete scheduler_entity->m_Task;
					scheduler_entity->m_Task = nullptr;
					delete scheduler_entity;
					scheduler_entity = nullptr;
				}
				else
				{
					++it;
				}
			}
		}
		
	}

	void CThreadTaskScheduler::ProcessDelayedTasks()
	{
		for (auto it = m_DelayedTasks.begin(); it != m_DelayedTasks.end();)
		{
			SThreadTaskSchedulerEntity* scheduler_entity = (*it);
			if (!scheduler_entity->m_Task->GetState()->m_Delayed)
			{
				it = m_DelayedTasks.erase(it);
				m_PendingTasks[scheduler_entity->m_Priority].push_back(scheduler_entity);
			}
		}

		for (auto &[priority, tasks] : m_PendingTasks)
		{
			for (auto it = tasks.begin(); it != tasks.end();)
			{
				SThreadTaskSchedulerEntity* scheduler_entity = (*it);
				if (scheduler_entity->m_Task->GetState()->m_Delayed.load())
				{
					it = tasks.erase(it);
					m_DelayedTasks.push_back(scheduler_entity);
				}
				else
				{
					++it;
				}
			}
		}
	} 

	void CThreadTaskScheduler::ProcessCompletedTasks()
	{
		for (SThreadTaskSchedulerEntity* scheduler_entity : m_CompletedTasks)
		{
			if (scheduler_entity->m_Task->GetState()->m_Completed.load())
			{
				delete scheduler_entity->m_Task;
				scheduler_entity->m_Task = nullptr;
				delete scheduler_entity;
				scheduler_entity = nullptr;
			}
		}
	}

	void CThreadTaskScheduler::ProcessPendingTasks()
	{
		std::vector<SThreadTaskSchedulerEntity*> tasks_to_enqueue;

		SThreadTaskSchedulerSingleProcessData single_proc_data;
		FillSingleProcessData(single_proc_data);

		FillTasksToEnqueue(tasks_to_enqueue, ETaskPriority::High);

		if (single_proc_data.m_ProcessNormalPriorityTasks)
		{
			FillTasksToEnqueue(tasks_to_enqueue, ETaskPriority::Normal);
		}

		if (single_proc_data.m_ProcessLowPriorityTasks)
		{
			FillTasksToEnqueue(tasks_to_enqueue, ETaskPriority::Low);
		}

		for (SThreadTaskSchedulerEntity* scheduler_entity : tasks_to_enqueue)
		{
			scheduler_entity->m_Task->GetState()->m_Started.store(true);
			m_ThreadPool.EnqueueTask(scheduler_entity->m_Task);
		}
	}
}

