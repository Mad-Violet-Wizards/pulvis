#include "FastFunction.hpp"

namespace pulvis::tl
{
	FastFunction::~FastFunction()
	{
		Reset();
	}

	FastFunction::FastFunction(FastFunction&& _other) noexcept
	{
		MoveFrom(std::move(_other));
	}

	FastFunction& FastFunction::operator=(FastFunction&& _other) noexcept
	{
		if (this != &_other)
		{
			Reset();
			MoveFrom(std::move(_other));
		}

		return *this;
	}

	void FastFunction::Reset()
	{
		if (m_Destroy) m_Destroy(m_Buffer);
		m_Invoke = nullptr;
		m_Destroy = nullptr;
		m_Move = nullptr;
	}

	void FastFunction::MoveFrom(FastFunction&& _other)
	{
		m_Invoke = _other.m_Invoke;
		m_Destroy = _other.m_Destroy;
		m_Move = _other.m_Move;

		if (m_Move)
		{
			m_Move(m_Buffer, _other.m_Buffer);
		}

		_other.Reset();
	}
}
