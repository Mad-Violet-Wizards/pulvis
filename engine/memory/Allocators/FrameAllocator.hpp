#pragma once

#include <memory>
#include <iomanip>

namespace engine::memory
{
	template<typename T>
	class FrameAllocator
	{
	public:

		FrameAllocator(EMemoryCategory _mem_category, size_t _capacity)
			: m_MemoryCategory(_mem_category)
			, m_Capacity(_capacity)
			, m_Offset(0)
		{
			m_Memory = engine::memory::Allocate<std::byte>(_mem_category, _capacity);
			std::memset(m_Memory, 0, _capacity);
		}

		template<typename U>
		FrameAllocator(const FrameAllocator<U>& _allocator) noexcept = delete;

		template<typename U>
		FrameAllocator& operator=(const FrameAllocator<U>& _allocator) noexcept = delete;

		template<typename U>
		FrameAllocator(FrameAllocator<U>&& _allocator) noexcept
		{
			m_MemoryCategory = _allocator.GetMemoryCategory();
			m_Capacity = _allocator.GetCapacity();
			m_Offset = _allocator.GetOffset();
			m_Memory = _allocator.GetMemory();

			_allocator.m_Memory = nullptr;
			_allocator.m_Capacity = 0;
			_allocator.m_Offset = 0;
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

				_allocator.m_Memory = nullptr;
				_allocator.m_Capacity = 0;
				_allocator.m_Offset = 0;
			}

			return *this;
		}

		~FrameAllocator() noexcept
		{
			engine::memory::Deallocate(m_MemoryCategory, m_Memory);
		}

		template<typename U>
		[[nodiscard]] U* Allocate(size_t _size)
		{
			static_assert(std::is_same_v<U, T>, "FrameAllocator: U must be the same type as T!");

			if (m_Offset == m_Capacity)
			{
				ASSERT(false, "FrameAllocator: ran out of memory!");
				return nullptr;
			}

			U* ptr = reinterpret_cast<T*>(m_Memory + m_Offset);
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

		template<typename U, typename... Args>
		void construct(U* _ptr, Args&&... _args) const noexcept
		{
			new (_ptr) U(std::forward<Args>(_args)...);
		}

		template<typename U>
		void destroy(U* _ptr) const noexcept
		{
			_ptr->~U();
		}

		bool operator==(const FrameAllocator& _rhs) const noexcept
		{
			return true;
		}

		bool operator!=(const FrameAllocator& _rhs) const noexcept
		{
			return !(*this == _rhs);
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