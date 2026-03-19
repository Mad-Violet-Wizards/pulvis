#pragma once

#include <atomic>
#include <cstdint>
#include <cstring>
#include <vector>
#include <type_traits>


/*
	Lock-free single-producer single-consumer ring buffer.

	Capacity is fixed at construction and must be a power of 2.

	The producer only writes to m_Head.
	The consumer only writes to m_Tail.
	Both lives on separate cache lines to avoid false sharing.

	Typical usage:
		RingBuffer buffer(65536);

		// Producer
		buffer.Push<SRenderCommand>(cmd);

		// Consumer
		SRenderCommand cmd;
		if (buffer.Pop(cmd))
		{
			// Process cmd
		}
*/
namespace pulvis::tl
{
	static constexpr size_t CACHE_LINE_SIZE = 64; // [bytes]
	static constexpr size_t RING_BUFFER_MAX_CAPACITY = 65536; // [bytes]

	class RingBuffer
	{
		public:

			explicit RingBuffer(size_t _capacity = RING_BUFFER_MAX_CAPACITY);

			RingBuffer(const RingBuffer& _other) = delete;
			RingBuffer& operator=(const RingBuffer& _other) = delete;

			RingBuffer(RingBuffer&& _other) noexcept;
			RingBuffer& operator=(RingBuffer&& _other) noexcept;

			template<typename T>
			bool Push(const T& _value)
			{
				static_assert(std::is_trivially_copyable_v<T>, "RingBuffer only supports trivially copyable types.");
				return PushBytes(&_value, sizeof(T));
			}

			template<typename T>
			bool Pop(T& _value)
			{
				static_assert(std::is_trivially_copyable_v<T>, "RingBuffer only supports trivially copyable types.");
				return PopBytes(&_value, sizeof(T));
			}

			bool PushBytes(const void* _data, size_t _size);
			bool PopBytes(void* _data, size_t _size);

			size_t GetCapacity() const { return m_Capacity; }
			size_t GetAvailableRead() const;
			size_t GetAvailableWrite() const;

			bool Empty() const { return GetAvailableRead() == 0; }

			void Reset();

		private:

			size_t m_Capacity;
			size_t m_Mask;
			std::vector<std::byte> m_Buffer;

			alignas(CACHE_LINE_SIZE) std::atomic<size_t> m_Head;
			alignas(CACHE_LINE_SIZE) std::atomic<size_t> m_Tail;
	};
}