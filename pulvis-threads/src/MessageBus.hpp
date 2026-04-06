#pragma once

#include "ThreadTypeTraits.hpp"

#include "RingBuffer.hpp"
#include "FastFunction.hpp"
#include "Hash.hpp"

#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <mutex>
#include <type_traits>

namespace pulvis::threads
{
	/*
		Header prepended to every message in the bus ring buffers.
		Uses a type hash to identify the message type at the receiver,
		followed by the byte size of the payload.
	*/
	struct SMessageHeader
	{
		uint32_t TypeHash;
		uint32_t Size;
	};

	template<typename T>
	uint32_t GetMessageTypeHash()
	{
#if defined(WINDOWS_OS)
		return tl::hash::fnv1a<uint32_t>(__FUNCSIG__);
#elif defined(MAC_OS)
		return tl::hash::fnv1a<uint32_t>(__PRETTY_FUNCTION__);
#endif
		return UINT32_MAX;
	}

	/*
		Thread-safe message bus.

		Each registered channel gets its own RingBuffer and set of typed handlers.
		Messages are type-erased and identified by compile-time hash derived from struct type.

		Handlers are storted as FastFunction to avoid heap allocation.
		Each handler has a fixed signature:
		void(const void* _data, uint32_t _size)

		Constraints on registered handlers:
			- Lambda with 2+ pointer captures: may exceed 24 bytes buffer.
			Pass a pointer to an external object instead.

		Channel IDs are arbitrary uint32_t values - typically one per
		dedicated thread (e.g. 0 = Main, 1 = Render, 2 = Audio, 3 = IO).

		Typical usage:
			CMessageBus bus;
			bus.RegisterChannel(CHANNEL_AUDIO, 65536);

			bus.RegisterHandler<SAudioPlayCmd>(CHANNEL_AUDIO,
				+[](const SAudioPlayCmd& _cmd) { play(_cmd); });

			// Producer (main thread):
			bus.Send<SAudioPlayCmd>(CHANNEL_AUDIO, cmd);

			// Consumer (audio thread, each iteration):
			bus.Drain(CHANNEL_AUDIO);
	*/

	class CMessageBus
	{
		public:

			CMessageBus() = default;
			~CMessageBus() = default;

			CMessageBus(const CMessageBus& _other) = delete;
			CMessageBus& operator=(const CMessageBus& _other) = delete;

			void RegisterChannel(uint32_t _channelId, size_t _bufferSize = DEFAULT_RING_BUFFER_CAPACITY);

			template<typename T, typename Func>
			void RegisterHandler(uint32_t _channelId, Func&& _handler)
			{
				static_assert(std::is_trivially_copyable_v<T>, "Message type must be trivially copyable.");

				struct SHandlerWrapper
				{
					std::decay_t<Func> m_UserHandler;

					void operator()(const void* _data, uint32_t _size)
					{
						T msg;
						std::memcpy(&msg, _data, sizeof(T));
						m_UserHandler(msg);
					}
				};

				static_assert(sizeof(SHandlerWrapper) <= 24,
					"Handler callable too large for FastFunction inline buffer (24B). "
					"Capture a pointer to external state instead of copying large objects.");

				const uint32_t type_hash = GetMessageTypeHash<T>();
				auto handler_wrapper = SHandlerWrapper{ std::forward<Func>(_handler) };

				std::lock_guard lock(m_RegistrationMutex);
				m_Handlers[_channelId][type_hash] = tl::FastFunction::Make<void, const void*, uint32_t>(std::move(handler_wrapper));
			}

			template<typename T>
			bool Send(uint32_t _channelId, const T& _message)
			{
				static_assert(std::is_trivially_copyable_v<T>, "Message type must be trivially copyable.");

				auto it = m_Channels.find(_channelId);
				if (it == m_Channels.end())
				{
					return false;
				}

				SMessageHeader header{ GetMessageTypeHash<T>(), sizeof(T) };

				tl::RingBuffer& buffer = it->second;

				if (!buffer.Push(header))
				{
					return false;
				}

				if (!buffer.PushBytes(&_message, sizeof(T)))
				{
					return false;
				}

				return true;
			}

			void Drain(uint32_t _channelId);

		private:

			std::unordered_map<uint32_t, tl::RingBuffer> m_Channels;
			std::unordered_map<uint32_t, std::unordered_map<uint32_t, tl::FastFunction>> m_Handlers;
			std::mutex m_RegistrationMutex;

			static constexpr size_t MAX_MESSAGE_SIZE = 1024; // bytes.
			std::byte m_MessageBuffer[MAX_MESSAGE_SIZE];
	};
}