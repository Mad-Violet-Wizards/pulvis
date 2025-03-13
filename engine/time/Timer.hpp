#pragma once

#include "TimeUnit.hpp"

#ifdef WINDOWS_OS
    #include <windows.h>
#else
    #include <ctime>
    #ifdef MAC_OS
        #include <mach/clock.h>
        #include <mach/mach.h>
    #endif
#endif

namespace engine::time
{
  class Timer
  {
		public:

			Timer();
			~Timer() = default;

			void Start();
			void End();
			void Reset();
			double GetElapsedTime(ETimeUnit _time_unit) const;

    private:

      bool m_bStarted;
#ifdef WINDOWS_OS
    LARGE_INTEGER m_Frequency;
    LARGE_INTEGER m_StartTime;
    LARGE_INTEGER m_EndTime;
#else
    struct timespec m_StartTime;
    struct timespec m_EndTime;
#endif
  };
}