#pragma once

#include <memory>

#include "Allocator.hpp"

#include "engine/memory/MacroUtility.hpp"


namespace engine::memory
{
	template<typename T>
	class FrameAllocator : public IAllocator
	{
		public:

			explicit FrameAllocator(EMemoryCategory _mem_category, size_t _capacity) noexcept
			{
				m_Capacity = _capacity;
				m_Offset = 0;
				m_MemoryCategory = _mem_category;
				m_Memory = engine::memory::Allocate<T>(_mem_category, _capacity);
			}

			~FrameAllocator() noexcept
			{
				Release();
			}

			[[nodiscard]] T* Allocate(size_t _n) noexcept
			{
				return static_cast<T*>(Alloc(_n));
			}

			void Deallocate(T* _ptr, size_t _n) noexcept
			{
				Dealloc(_ptr, _n);
			}

			[[nodiscard]] void* Alloc(size_t _size) noexcept
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
				return raw_ptr;
			}

			void Dealloc(void* _ptr, size_t _n) noexcept
			{
				// Do nothing.
			}

			void Reset() noexcept override
			{
				m_Offset = 0;
			}

			void Release() noexcept override
			{
				if (m_Memory)
				{
					engine::memory::Deallocate(m_MemoryCategory, m_Memory);
					m_Memory = nullptr;
					m_Offset = 0;
				}
			}

			void ZeroMemory() noexcept override
			{
				std::memset(m_Memory, 0, m_Capacity);
			}

		private:

			size_t m_Capacity;
			size_t m_Offset;
			EMemoryCategory m_MemoryCategory;
			void* m_Memory;
	};
}