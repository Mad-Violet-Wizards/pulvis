#pragma once

#include <memory>
#include <iomanip>

#include "engine/memory/MacroUtility.hpp"


namespace engine::memory
{
	template<typename T>
	class FrameAllocator
	{
	public:

		ALLOCATOR_API(FrameAllocator, T);

		FrameAllocator(EMemoryCategory _mem_category, size_t _capacity)
			: m_MemoryCategory(_mem_category)
			, m_Capacity(_capacity)
			, m_Offset(0)
		{
			m_Memory = engine::memory::Allocate<std::byte>(_mem_category, _capacity);
			std::memset(m_Memory, 0, _capacity);
		}

		template<typename U>
		FrameAllocator(const FrameAllocator<U>& _allocator) noexcept
		{
			m_MemoryCategory = _allocator.GetMemoryCategory();
			m_Capacity = _allocator.GetCapacity();
			m_Offset = _allocator.GetOffset();
			m_Memory = _allocator.GetMemory();
		}

		template<typename U>
		FrameAllocator& operator=(const FrameAllocator<U>& _allocator) noexcept
		{
			if (this != &_allocator)
			{
				m_MemoryCategory = _allocator.GetMemoryCategory();
				m_Capacity = _allocator.GetCapacity();
				m_Offset = _allocator.GetOffset();
				m_Memory = _allocator.GetMemory();
			}

			return *this;
		}

		template<typename U>
		FrameAllocator(FrameAllocator<U>&& _allocator) noexcept
		{
			m_MemoryCategory = _allocator.GetMemoryCategory();
			m_Capacity = _allocator.GetCapacity();
			m_Offset = _allocator.GetOffset();
			m_Memory = _allocator.GetMemory();
		}

		template<typename U>
		FrameAllocator& operator=(FrameAllocator<U>&& _allocator) noexcept
		{
			if (this != &_allocator)
			{
				m_MemoryCategory = _allocator.GetMemoryCategory();
				m_Capacity = _allocator.GetCapacity();
				m_Offset = _allocator.GetOffset();
				m_Memory = _allocator.GetMemory();
			}

			return *this;
		}

		~FrameAllocator() noexcept
		{
			engine::memory::Deallocate(m_MemoryCategory, m_Memory);
		}

		[[nodiscard]] T* Allocate(size_t _size)
		{
			if (m_Offset + _size > m_Capacity)
			{
				ASSERT(false, "FrameAllocator: ran out of memory!");
				return nullptr;
			}

			T* ptr = reinterpret_cast<T*>(m_Memory + m_Offset);
			m_Offset += _size;
			return ptr;
		}

		void Deallocate(T* _ptr, size_t _n)
		{
			// Do nothing.
		}

		EMemoryCategory GetMemoryCategory() const
		{
			return m_MemoryCategory;
		}

		std::byte* GetMemory() const
		{
			return m_Memory;
		}

		size_t GetCapacity() const
		{
			return m_Capacity;
		}

		size_t GetOffset() const
		{
			return m_Offset;
		}

		void DumpConsole()
		{
			std::cout << "[FrameAllocator] Dumping memory: \n";
			std::cout << "Capacity: " << m_Capacity << " | Offset: " << m_Offset << "\n";
			std::cout << "Memory:\n";

			for (size_t i = 0; i < m_Capacity; ++i)
			{
				std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(m_Memory[i]) << " ";
			}
			std::cout << std::dec << std::endl;
		}

	private:

		EMemoryCategory m_MemoryCategory;
		size_t m_Capacity;
		size_t m_Offset;
		std::byte* m_Memory{ nullptr };
	};
}