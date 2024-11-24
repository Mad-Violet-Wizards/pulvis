#pragma once

#include <memory>

#include "engine/memory/MemoryCategory.hpp"
#include "engine/memory/Memory.hpp"

#include "Allocator.hpp"

namespace engine::memory
{
	template<class T>
	class FrameAllocator : public IAllocator<T>
	{
		public:

			FrameAllocator(EMemoryCategory _mem_category, size_t _capacity) noexcept
				: m_Capacity(_capacity)
				, m_Offset(0)
			{
				void* raw_memory = Allocate(_mem_category, _capacity);
				m_Memory = static_cast<T*>(raw_memory);
			}

			~FrameAllocator() noexcept
			{
				Release();
			}

			[[nodiscard]] IAllocator<T>::pointer allocate(IAllocator<T>::size_type _size) noexcept
			{
				if (!m_Memory)
				{
					return nullptr;
				}

				if (m_Offset + _size > m_Capacity)
				{
					ASSERT(false, "FrameAllocator: ran out of memory!");
					return nullptr;
				}
				
				void* raw_ptr = static_cast<char*>(m_Memory) + m_Offset;
				size_t remaining_size = m_Capacity - m_Offset;

				if (!std::align(alignof(T), _size, raw_ptr, remaining_size))
				{
					ASSERT(false, "FrameAllocator: can't align pointer!");
					return nullptr;
				}

				m_Offset += _size;
				return static_cast<IAllocator<T>::pointer>(raw_ptr);
			}

			void deallocate(IAllocator<T>::pointer _ptr, IAllocator<T>::size_type _n) noexcept
			{
				// Do nothing.
			}

			void Reset() noexcept
			{
				m_Offset = 0;
			}

			void Release() noexcept
			{
				if (m_Memory)
				{
					std::free(m_Memory);
					m_Memory = nullptr;
					m_Offset = 0;
				}
			}

			void ZeroMemory() noexcept
			{
				memset(m_Memory, 0, m_Capacity);
			}

		private:

			void* m_Memory;
			size_t m_Capacity;
			size_t m_Offset;


	};
}