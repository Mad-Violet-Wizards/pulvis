#pragma once

#include <atomic>
#include <cstdint>
#include <vector>

/* 
	Lock-free dequeue based on Chase-Lev algorithm.

	The owner thread pushes and pops from the bottom (LIFO)
	while the other thread steal from the top (FIFO).

	Push / Pop - called only by the owning thread
	Steal - called only by any other thread

	Capacity is fixed at construction and must be a power of 2.

	Typical usage:
		StealDequeue<SJob*> queue(1024);
		queue.Push(job); // owner
		SJob* job = queue.Pop(); // owner
		SJob* stolen_job = queue.Steal(); //thief
*/
namespace pulvis::tl
{
	template<typename T>
	class StealingDequeue
	{
	public:

		explicit StealingDequeue(size_t capacity)
			: m_Capacity(capacity)
			, m_Mask(capacity - 1)
			, m_Buffer(capacity)
			, m_Bottom(0)
			, m_Top(0)
		{
		}

		StealingDequeue(const StealingDequeue& _other) = delete;
		StealingDequeue& operator=(const StealingDequeue& _other) = delete;

		StealingDequeue(StealingDequeue&& _other) noexcept
			: m_Capacity(_other.m_Capacity)
			, m_Mask(_other.m_Mask)
			, m_Buffer(std::move(_other.m_Buffer))
			, m_Bottom(_other.m_Bottom.load(std::memory_order_relaxed))
			, m_Top(_other.m_Top.load(std::memory_order_relaxed))
		{
			_other.m_Capacity = 0;
			_other.m_Mask = 0;
			_other.m_Bottom.store(0, std::memory_order_relaxed);
			_other.m_Top.store(0, std::memory_order_relaxed);
		}

		StealingDequeue& operator=(StealingDequeue&& _other) noexcept
		{
			if (this != &_other)
			{
				m_Capacity = _other.m_Capacity;
				m_Mask = _other.m_Mask;
				m_Buffer = std::move(_other.m_Buffer);
				m_Bottom.store(_other.m_Bottom.load(std::memory_order_relaxed), std::memory_order_relaxed);
				m_Top.store(_other.m_Top.load(std::memory_order_relaxed), std::memory_order_relaxed);

				_other.m_Capacity = 0;
				_other.m_Mask = 0;
				_other.m_Bottom.store(0, std::memory_order_relaxed);
				_other.m_Top.store(0, std::memory_order_relaxed);
			}

			return *this;
		}

		bool Push(T _value)
		{
			const int64_t bottom = m_Bottom.load(std::memory_order_relaxed);
			const int64_t top = m_Top.load(std::memory_order_acquire);

			if (bottom - top >= static_cast<int64_t>(m_Capacity))
			{
				return false; // Full
			}

			m_Buffer[static_cast<size_t>(bottom & m_Mask)] = _value;
			std::atomic_thread_fence(std::memory_order_release);
			m_Bottom.store(bottom + 1, std::memory_order_relaxed);

			return true;
		}
		T Pop()
		{
			const int64_t bottom = m_Bottom.load(std::memory_order_relaxed) - 1;
			m_Bottom.store(bottom, std::memory_order_relaxed);
			std::atomic_thread_fence(std::memory_order_seq_cst);

			const int64_t top = m_Top.load(std::memory_order_relaxed);

			if (top <= bottom)
			{
				T value = m_Buffer[static_cast<size_t>(bottom & m_Mask)];

				if (top != bottom)
				{
					return value;
				}

				// Top == bottom -> We might have a race between owner & thief -> Try to claim it with CAS.
				int64_t expected_top = top;
				if (!m_Top.compare_exchange_strong(expected_top, top + 1,
					std::memory_order_seq_cst,
					std::memory_order_relaxed))
				{
					value = T{};
				}

				m_Bottom.store(top + 1, std::memory_order_relaxed);
				return value;
			}

			// Queue was empty restore bottom.
			m_Bottom.store(top, std::memory_order_relaxed);
			return T{};
		}

		T Steal()
		{
			int64_t top = m_Top.load(std::memory_order_acquire);
			std::atomic_thread_fence(std::memory_order_seq_cst);
			int64_t bottom = m_Bottom.load(std::memory_order_acquire);

			if (top >= bottom)
			{
				return T{};
			}

			T value = m_Buffer[static_cast<size_t>(top & m_Mask)];

			// If other thread has stolen the same item faster -> top changed -> CAS will fail.
			if (!m_Top.compare_exchange_strong(top, top + 1,
				std::memory_order_seq_cst,
				std::memory_order_relaxed))
			{
				return T{};
			}

			return value;
		}


		int64_t Size() const
		{
			int64_t bottom = m_Bottom.load(std::memory_order_relaxed);
			int64_t top = m_Top.load(std::memory_order_relaxed);
			return bottom - top;
		}

		bool Empty() const
		{
			return Size() <= 0;
		}

	private:

		std::atomic<int64_t> m_Bottom;
		std::atomic<int64_t> m_Top;
		size_t m_Capacity;
		size_t m_Mask;
		std::vector<T> m_Buffer;
	};
}
