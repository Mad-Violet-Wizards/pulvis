#include "MessageBus.hpp"

namespace pulvis::threads
{
	void CMessageBus::RegisterChannel(uint32_t _channelId, size_t _bufferSize)
	{
		std::lock_guard lock(m_RegistrationMutex);
		m_Channels.emplace(_channelId, tl::RingBuffer(_bufferSize));
	}

	void CMessageBus::Drain(uint32_t _channelId)
	{
		auto channel_it = m_Channels.find(_channelId);
		if (channel_it == m_Channels.end())
		{
			return;
		}

		tl::RingBuffer& buffer = channel_it->second;
		auto handlers_it = m_Handlers.find(_channelId);
		if (handlers_it == m_Handlers.end())
		{
			return;
		}

		auto& handlers = handlers_it->second;

		while (!buffer.Empty())
		{
			SMessageHeader header;
			if (!buffer.Pop(header))
			{
				break;
			}

			if (header.Size > MAX_MESSAGE_SIZE)
			{
				uint8_t discard[256];
				size_t remaining = header.Size;
				while (remaining > 0)
				{
					size_t chunk = (remaining < sizeof(discard)) ? remaining : sizeof(discard);
					buffer.PopBytes(discard, chunk);
					remaining -= chunk;
				}
				continue;
			}

			if (!buffer.PopBytes(m_MessageBuffer, header.Size))
			{
				break;
			}

			auto handler_it = handlers.find(header.TypeHash);
			if (handler_it != handlers.end())
			{
				handler_it->second.Invoke<void>(
					static_cast<const void*>(m_MessageBuffer),
					header.Size);
			}
		}
	}
}