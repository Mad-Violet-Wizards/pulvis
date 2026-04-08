#include "MessageBus.hpp"

namespace pulvis::threads
{
	uint32_t CMessageBus::RegisterChannel(size_t _bufferSize)
	{
		std::lock_guard lock(m_RegistrationMutex);
		const uint32_t new_channel_id = m_NextChannelId;
		m_Channels.emplace(new_channel_id, tl::RingBuffer(_bufferSize));
		m_NextChannelId++;
		return new_channel_id;
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