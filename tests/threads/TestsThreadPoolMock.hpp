#pragma once

#include <chrono>
#include <thread>

namespace tests::threads
{
	static constexpr int s_MocReturnValue = 42;

  void MocVoid();
  int MocAddTwoValues(int _a, int _b);
  void RefAddTwoValues(int& _a, int& _b, int& _res);
  int MocNoSleepReturn();

  int MocSleepThreadMiliseconds(int _milliseconds = 100);
  int MocSleepThreadSeconds(int _seconds = 1);

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