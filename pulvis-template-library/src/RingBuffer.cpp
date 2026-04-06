#include "RingBuffer.hpp"

namespace pulvis::tl
{
	RingBuffer::RingBuffer(size_t _capacity)
		: m_Capacity(_capacity)
		, m_Mask(_capacity - 1)
		, m_Buffer(_capacity)
		, m_Head(0)
		, m_Tail(0)
	{
		m_Head.store(0, std::memory_order_relaxed);
		m_Tail.store(0, std::memory_order_relaxed);
	}

	RingBuffer::RingBuffer(RingBuffer&& _other) noexcept
		: m_Capacity(_other.m_Capacity)
		, m_Mask(_other.m_Mask)
		, m_Buffer(std::move(_other.m_Buffer))
	{
		m_Head.store(_other.m_Head.load(std::memory_order_relaxed), std::memory_order_relaxed);
		m_Tail.store(_other.m_Tail.load(std::memory_order_relaxed), std::memory_order_relaxed);

		_other.m_Capacity = 0;
		_other.m_Mask = 0;
		_other.m_Head.store(0, std::memory_order_relaxed);
		_other.m_Tail.store(0, std::memory_order_relaxed);
	}

	RingBuffer& RingBuffer::operator=(RingBuffer&& _other) noexcept
	{
		if (this != &_other)
		{
			m_Capacity = _other.m_Capacity;
			m_Mask = _other.m_Mask;
			m_Buffer = std::move(_other.m_Buffer);
			m_Head.store(_other.m_Head.load(std::memory_order_relaxed), std::memory_order_relaxed);
			m_Tail.store(_other.m_Tail.load(std::memory_order_relaxed), std::memory_order_relaxed);

			_other.m_Capacity = 0;
			_other.m_Mask = 0;
			_other.m_Head.store(0, std::memory_order_relaxed);
			_other.m_Tail.store(0, std::memory_order_relaxed);
		}

		return *this;
	}

	bool RingBuffer::PushBytes(const void* _data, size_t _size)
	{
		const size_t head = m_Head.load(std::memory_order_relaxed);
		const size_t tail = m_Tail.load(std::memory_order_acquire);

		const size_t used = head - tail;
		if (used + _size > m_Capacity)
		{
			return false;
		}

		const size_t offset = head & m_Mask;
		const size_t first_chunk = m_Capacity - offset;
		const std::byte* src = static_cast<const std::byte*>(_data);

		if (_size <= first_chunk)
		{
			std::memcpy(m_Buffer.data() + offset, src, _size);
		}
		else
		{
			std::memcpy(m_Buffer.data() + offset, src, first_chunk);
			std::memcpy(m_Buffer.data(), src + first_chunk, _size - first_chunk);
		}

		m_Head.store(head + _size, std::memory_order_release);
		return true;
	}

	bool RingBuffer::PopBytes(void* _data, size_t _size)
	{
		 const size_t tail = m_Tail.load(std::memory_order_relaxed);
		 const size_t head = m_Head.load(std::memory_order_acquire);

		 const size_t available = head - tail;
		 if (available < _size)
		 {
			 return false;
		 }

		 const size_t offset = tail & m_Mask;
		 const size_t first_chunk = m_Capacity - offset;
		 std::byte* dst = static_cast<std::byte*>(_data);

		 if (_size <= first_chunk)
		 {
			 std::memcpy(dst, m_Buffer.data() + offset, _size);
		 }
		 else
		 {
			 std::memcpy(dst, m_Buffer.data() + offset, first_chunk);
			 std::memcpy(dst + first_chunk, m_Buffer.data(), _size - first_chunk);
		 }

		 m_Tail.store(tail + _size, std::memory_order_release);
		 return true;
	}

	size_t RingBuffer::GetAvailableRead() const
	{
		return m_Head.load(std::memory_order_acquire) - m_Tail.load(std::memory_order_relaxed);
	}

	size_t RingBuffer::GetAvailableWrite() const
	{
		return m_Capacity - GetAvailableRead();
	}

	void RingBuffer::Reset()
	{
		m_Head.store(0, std::memory_order_relaxed);
		m_Tail.store(0, std::memory_order_relaxed);
	}
}