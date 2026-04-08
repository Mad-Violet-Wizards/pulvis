#pragma once

#include <cstdint>
#include <functional>

namespace pulvis::events
{
//////////////////////////////////////////////////////////////////////////
	using listener_id_t = uint64_t;

//////////////////////////////////////////////////////////////////////////
	class CEventDispatcher;

	class CEventHandle
	{
		public:

			CEventHandle() = default;
			CEventHandle(CEventDispatcher* _dispatcher, listener_id_t _listenerId);
			~CEventHandle();

			CEventHandle(CEventHandle&& _other) noexcept;
			CEventHandle& operator=(CEventHandle&& _other) noexcept;

			CEventHandle(const CEventHandle&) = delete;
			CEventHandle& operator=(const CEventHandle&) = delete;

			void Reset();
			bool IsValid() const;

		private:

			CEventDispatcher* m_Dispatcher = nullptr;
			listener_id_t m_ListenerId = 0;
	};
}