#pragma once

#include "engine/threads/ThreadPool.hpp"
#include "engine/threads/ThreadTask.hpp"
#include "engine/threads/ThreadTaskHandle.hpp"
#include "engine/threads/ThreadTaskState.hpp"
#include "engine/threads/ThreadTaskScheduler.hpp"

namespace engine::threads
{
	namespace tests
	{
		static constexpr int s_MocReturnValue = 42;

		void MocVoid()
		{
			std::string str = "Hello World!";
			str[0] = 'h';
		}

		int MocAddTwoValues(int _a, int _b)
		{
			return _a + _b;
		}

		void RefAddTwoValues(int& _a, int& _b, int& _res)
		{
			_res = _a + _b;
		}

		int MocNoSleepReturn()
		{
			return s_MocReturnValue;
		}

		int MocSleepThread100ms()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			return s_MocReturnValue;
		}

		int MocSleepThread1s()
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			return s_MocReturnValue;
		}

		int MocSleepThread5s()
		{
			std::this_thread::sleep_for(std::chrono::seconds(5));
			return s_MocReturnValue;
		}

		class MocThreadNotSharedClass
		{
			public:

				void MocVoid()
				{
					std::string str = "Hello World!";
					str[0] = 'h';
				}

				int MocAddTwoValues(int _a, int _b)
				{
					return _a + _b;
				}

				void RefAddTwoValues(int& _a, int& _b, int& _res)
				{
					_res = _a + _b;
				}

				int MocNoSleepReturn() const
				{
					return s_MocReturnValue;
				}

				int MocSleepThread100ms() const
				{
					return s_MocReturnValue;
				}

				int MocSleepThread1s() const
				{
					return s_MocReturnValue;
				}

				int MocSleepThread5s() const
				{
					return s_MocReturnValue;
				}
		};

		class MocThreadSharedClass
		{
			public:

				std::vector<int> GetToAccumulate() const
				{
					std::vector<int> toAccumulate;
					for(int i = 0; i < 50000; ++i)
					{
						toAccumulate.push_back(1);
					}
					return toAccumulate;
				}

				void Accumulate(const std::vector<int>& _toAccumulate)
				{
					for (int value : _toAccumulate)
					{
						m_Accumulated += value;
					}
				}

		private:

			int m_Accumulated = 0;
		};
	}
} // namespace engine::threading