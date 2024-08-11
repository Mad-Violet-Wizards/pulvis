#include "engine/engine_pch.hpp"

#ifdef MAC_OS
#include "Timer.hpp"



namespace engine
{
namespace time
{
    void Timer::Start()
    {
        m_bStarted = true;
        clock_serv_t clock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock);
        clock_get_time(clock, &mts);
        mach_port_deallocate(mach_task_self(), clock);
        m_StartTime.tv_sec = mts.tv_sec;
        m_StartTime.tv_nsec = mts.tv_nsec;
    }

    void Timer::End()
    {
        ASSERT(m_bStarted, "You have to start the clock first!");
        clock_serv_t clock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &clock);
        clock_get_time(clock, &mts);
        mach_port_deallocate(mach_task_self(), clock);
        m_EndTime.tv_sec = mts.tv_sec;
        m_EndTime.tv_nsec = mts.tv_nsec;
    }

    void Timer::Reset()
    {
        m_EndTime = m_StartTime;
    }

    double Timer::GetElapsedTime(ETimeUnit _time_unit) const
    {
        const long seconds = m_EndTime.tv_sec - m_StartTime.tv_sec;
        const long nanoseconds = m_EndTime.tv_nsec - m_StartTime.tv_nsec;
        const double elapsed = seconds + nanoseconds * 1e-9;

        switch(_time_unit)
        {
            case ETimeUnit::Seconds: return elapsed;
            case ETimeUnit::Milliseconds: return elapsed * 1000.0;
            case ETimeUnit::Microseconds: return elapsed * 1000000.0;
            case ETimeUnit::Nanoseconds: return elapsed * 1000000000.0;
            default:
            {
                ASSERT(true == false, "Unrecognized time unit - returning milliseconds as default");
                return elapsed * 1000.0;
            }
        }
    }
}
}
#endif