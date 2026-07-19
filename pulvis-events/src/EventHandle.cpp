#include "EventHandle.hpp"
#include "EventDispatcher.hpp"

namespace pulvis::events
{
	CEventHandle::CEventHandle(std::weak_ptr<CEventDispatcher> _dispatcher, listener_id_t _listenerId)
		: m_Dispatcher(_dispatcher)
		, m_ListenerId(_listenerId)
	{
	}

	CEventHandle::~CEventHandle()
	{
		Reset();
	}

	CEventHandle::CEventHandle(CEventHandle&& _other) noexcept
		: m_Dispatcher(_other.m_Dispatcher)
		, m_ListenerId(_other.m_ListenerId)
	{
		_other.m_Dispatcher.reset();
		_other.m_ListenerId = 0;
	}

	CEventHandle& CEventHandle::operator=(CEventHandle&& _other) noexcept
	{
		if (this != &_other)
		{
			Reset();
			m_Dispatcher = std::move(_other.m_Dispatcher);
			m_ListenerId = _other.m_ListenerId;
			_other.m_Dispatcher.reset();
			_other.m_ListenerId = 0;
		}
		return *this;
	}

	void CEventHandle::Reset()
	{
		if (auto dispatcher = m_Dispatcher.lock())
		{
			dispatcher->Unsubscribe(m_ListenerId);
		}

		m_Dispatcher.reset();
		m_ListenerId = 0;
	}

	bool CEventHandle::IsValid() const
	{
		return !m_Dispatcher.expired() && m_ListenerId != 0;
	}
}