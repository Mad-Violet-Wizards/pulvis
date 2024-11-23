#include "engine/engine_pch.hpp"

#ifdef WINDOWS_OS

#include "Timer.hpp"


namespace engine
{
	namespace time
	{
		void Timer::Start()
		{
			QueryPerformanceFrequency(&m_Frequency);
			QueryPerformanceCounter(&m_StartTime);
		}

		void Timer::End()
		{
			QueryPerformanceCounter(&m_EndTime);
		}

		double Timer::GetElapsedTime(ETimeUnit _time_unit) const 
		{
			const double elapsed = (m_EndTime.QuadPart - m_StartTime.QuadPart) / static_cast<double>(m_Frequency.QuadPart);

			switch (_time_unit)
			{
				case ETimeUnit::Seconds: return elapsed;
				case ETimeUnit::Milliseconds: return elapsed * 1000.0;
				case ETimeUnit::Microseconds: return elapsed * 1000000.0;
				case ETimeUnit::Nanoseconds: return elapsed * 1000000000.0;
			}

			return 0.0;
		}

		void Timer::Reset()
		{
			m_EndTime = m_StartTime;
		}


	}
}


#endif