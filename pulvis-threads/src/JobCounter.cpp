#include "JobCounter.hpp"

namespace pulvis::threads
{
	CJobCounter::CJobCounter()
		: m_Count(0)
	{
	}

	CJobCounter::CJobCounter(int32_t _initial_count)
		: m_Count(_initial_count)
	{
	}

	void CJobCounter::Increment(int32_t _value)
	{
		m_Count.fetch_add(_value, std::memory_order_release);
	}

	bool CJobCounter::Decrement(int32_t _value)
	{
		return m_Count.fetch_sub(_value, std::memory_order_acq_rel) == _value;
	}

	int32_t CJobCounter::Get() const
	{
		return m_Count.load(std::memory_order_acquire);
	}

	bool CJobCounter::IsReady() const
	{
		return m_Count.load(std::memory_order_acquire) <= 0;
	}

	void CJobCounter::operator+=(int32_t _value)
	{
		Increment(_value);
	}

	void CJobCounter::operator-=(int32_t _value)
	{
		Decrement(_value);
	}

}
