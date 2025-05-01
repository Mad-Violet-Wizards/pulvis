#pragma once

#include "ThreadPool.hpp"
#include "ThreadTaskHandle.hpp"
#include "engine/core/Assert.hpp"

namespace engine::threads
{
	enum class ETaskPriority
	{
		Unknown,
		Low, // Lazy evaluated
		Normal, // Normal priority
		High, // Run as soon as possible
	};

	struct SThreadTaskSchedulerSingleProcessData
	{
		bool m_ProcessNormalPriorityTasks{ true };
		bool m_ProcessLowPriorityTasks{ true };
	};

	struct SThreadTaskSchedulerEntity
	{
		SThreadTaskSchedulerEntity(CThreadTask* _task, ETaskPriority _priority)
			: m_Task(_task), m_Priority(_priority) {}

		CThreadTask* m_Task{ nullptr };
		ETaskPriority m_Priority{ ETaskPriority::Unknown };
	};


	class CThreadTaskScheduler
	{
		public:

			CThreadTaskScheduler(CThreadPool& _threadPool);
			~CThreadTaskScheduler();

			CThreadTaskScheduler(const CThreadTaskScheduler&) = delete;
			CThreadTaskScheduler& operator=(const CThreadTaskScheduler&) = delete;

			CThreadTaskScheduler(CThreadTaskScheduler&& _other) = delete;
			CThreadTaskScheduler& operator=(CThreadTaskScheduler&& _other) = delete;

			template<typename R, typename C, typename... Args>
			std::shared_ptr<CThreadTaskHandle<R>> ScheduleTask(const char* _task_name,
																				ETaskPriority _priority, 
																				C* _instance, 
																				R(C::* _method)(Args...), 
																			  Args&&... _args)
			{
				CThreadTask* task_ptr = new CThreadTask(_task_name, _instance, _method, std::forward<Args>(_args)...);

				std::vector<SThreadTaskSchedulerEntity*>& tasks = m_PendingTasks[_priority];
				std::vector<SThreadTaskSchedulerEntity*>::iterator it = std::find_if(tasks.begin(), tasks.end(),
					[_task_name](const SThreadTaskSchedulerEntity* entity) 
					{ 
						return entity->m_Task->GetState()->m_TaskID == pstd::ConstexprStringHash(_task_name);
					});

				const std::string assert_msg = fmt::format("Task with the same name already exists in the scheduler. Task name: {}", _task_name);
				ASSERT(it == tasks.end(), assert_msg);

				if (it == tasks.end())
				{
					SThreadTaskSchedulerEntity* entity = new SThreadTaskSchedulerEntity(task_ptr, _priority);
					m_PendingTasks[_priority].push_back(entity);
				}

				return task_ptr->GetHandle<R>();
			}

			template<typename R, typename Func, typename... Args>
			std::shared_ptr<CThreadTaskHandle<R>> ScheduleTask(const char* _task_name, 
																			  ETaskPriority  _priority, 
																			  Func&& _func, 
																				Args&&... _args)
			{
				CThreadTask* task_ptr = new CThreadTask(_task_name, _func, std::forward<Args>(_args)...);

				std::vector<SThreadTaskSchedulerEntity*>& tasks = m_PendingTasks[_priority];
				std::vector<SThreadTaskSchedulerEntity*>::iterator it = std::find_if(tasks.begin(), tasks.end(),
					[_task_name](const SThreadTaskSchedulerEntity* entity)
					{
						return entity->m_Task->GetState()->m_TaskID == pstd::ConstexprStringHash(_task_name);
					});

				const std::string assert_msg = fmt::format("Task with the same name already exists in the scheduler. Task name: {}", _task_name);
				ASSERT(it == tasks.end(), assert_msg);

				if (it == tasks.end())
				{
					SThreadTaskSchedulerEntity* entity = new SThreadTaskSchedulerEntity(task_ptr, _priority);
					m_PendingTasks[_priority].push_back(entity);
				}

				return task_ptr->GetHandle<R>();
			}

			void Process();

			void ReleaseCompletedTask(const char* _task_name);

		private:

			void FillSingleProcessData(SThreadTaskSchedulerSingleProcessData& _single_proc_data);
			void FillTasksToEnqueue(std::vector<SThreadTaskSchedulerEntity*>& _tasks_to_enqueue, ETaskPriority _priority);

			void ProcessCancelledTasks();
			void ProcessDelayedTasks();
			void ProcessCompletedTasks();
			void ProcessPendingTasks();

		private:

			CThreadPool& m_ThreadPool;

			std::unordered_map<ETaskPriority, std::vector<SThreadTaskSchedulerEntity*>> m_PendingTasks;

			std::vector<SThreadTaskSchedulerEntity*> m_DelayedTasks;
			std::vector<SThreadTaskSchedulerEntity*> m_CompletedTasks;

	};
}