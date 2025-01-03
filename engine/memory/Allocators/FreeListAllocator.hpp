#pragma once

#include "engine/memory/MacroUtility.hpp"
#include "engine/memory/MemoryCategory.hpp"
#include <iomanip>

namespace engine::memory
{
	class FreeListAllocatorNode
	{
	public:

		FreeListAllocatorNode* m_Next{ nullptr };
		size_t m_Size{ 0 };
		size_t m_Offset{ 0 };
		bool m_IsFree{ true };
	};

	template<typename T>
	class FreeListAllocator
	{
		ALLOCATOR_API(FreeListAllocator, T);

	public:

		FreeListAllocator(EMemoryCategory _mem_category, size_t _capacity)
			: m_MemoryCategory(_mem_category)
			, m_Capacity(_capacity)
		{
			m_Head = new FreeListAllocatorNode();
			m_Head->m_Size = _capacity;
			m_Head->m_Offset = 0;
			m_Memory = engine::memory::Allocate<std::byte>(_mem_category, _capacity);
			std::memset(m_Memory, 0, _capacity);
		}

		template<typename U>
		FreeListAllocator(const FreeListAllocator<U>& _other) noexcept
		{
			m_MemoryCategory = _other.GetMemoryCategory();
			m_Capacity = _other.GetCapacity();
			m_Head = _other.GetHead();
			m_Memory = _other.GetMemory();
		}

		template<typename U>
		FreeListAllocator& operator=(const FreeListAllocator<U>& _other) noexcept
		{
			if (this != &_other)
			{
				m_MemoryCategory = _other.GetMemoryCategory();
				m_Capacity = _other.GetCapacity();
				m_Head = _other.GetHead();
				m_Memory = _other.GetMemory();
			}

			return *this;
		}

		template<typename U>
		FreeListAllocator(FreeListAllocator<U>&& _other) noexcept
		{
			m_MemoryCategory = _other.GetMemoryCategory();
			m_Capacity = _other.GetCapacity();
			m_Head = _other.GetHead();
			m_Memory = std::move(_other.GetMemory());

			_other.m_Head = nullptr;
			_other.m_Memory = nullptr;
			_other.m_Capacity = 0;
			_other.m_MemoryCategory = EMemoryCategory::Undefined;
		}

		template<typename U>
		FreeListAllocator& operator=(FreeListAllocator<U>&& _other) noexcept
		{
			if (this != &_other)
			{
				m_MemoryCategory = _other.GetMemoryCategory();
				m_Capacity = _other.GetCapacity();
				m_Head = _other.GetHead();
				m_Memory = std::move(_other.GetMemory());

				_other.m_Head = nullptr;
				_other.m_Memory = nullptr;
				_other.m_Capacity = 0;
				_other.m_MemoryCategory = EMemoryCategory::Undefined;
			}

			return *this;
		}

		~FreeListAllocator()
		{
			engine::memory::Deallocate(m_MemoryCategory, m_Memory);

			FreeListAllocatorNode* current = m_Head;
			FreeListAllocatorNode* next{ nullptr };
			FreeListAllocatorNode* previous{ nullptr };

			while (current)
			{
				next = current->m_Next;
				previous = current;
				current = next;

				if (previous)
				{
					delete previous;
					previous = nullptr;
				}
			}

			m_Head = nullptr;
		}

		T* Allocate(size_t _size)
		{
			FreeListAllocatorNode* current = m_Head;

			while (current)
			{
				if (current->m_IsFree && current->m_Size >= _size)
				{
						// Split the block
						FreeListAllocatorNode* new_node = new FreeListAllocatorNode();
						new_node->m_Size = current->m_Size - _size;
						new_node->m_Offset = current->m_Offset + _size;

						if (current->m_Next)
						{
							new_node->m_Next = current->m_Next;
						}

						current->m_Size = _size;
						current->m_Next = new_node;
						current->m_IsFree = false;
						return reinterpret_cast<T*>(m_Memory + current->m_Offset);
				}

				current = current->m_Next;
			}

			return nullptr;
		}

		void Deallocate(T* _ptr, size_t _size)
		{
			FreeListAllocatorNode* current = m_Head;
			FreeListAllocatorNode* previous = nullptr;

			while (current)
			{
				if (current->m_Size == _size && m_Memory + current->m_Offset == reinterpret_cast<std::byte*>(_ptr))
				{
					std::memset(m_Memory + current->m_Offset, 0, _size);
					current->m_IsFree = true;
					break;
				}

				previous = current;
				current = current->m_Next;
			}

			FindAndMergeFreeBlocks();
		}

		EMemoryCategory GetMemoryCategory() const
		{
			return m_MemoryCategory;
		}

		size_t GetCapacity() const
		{
			return m_Capacity;
		}

		FreeListAllocatorNode* GetHead() const
		{
			return m_Head;
		}

		std::byte* GetMemory() const
		{
			return m_Memory;
		}

#ifdef DEBUG
		void DumpConsole()
		{
			FreeListAllocatorNode* current = m_Head;

			while (current)
			{
				std::cout << "Offset: " << current->m_Offset << " | Size: " << current->m_Size << " | IsFree: " << current->m_IsFree;

				if (current->m_Next)
				{
					std::cout << " -->\n";
				}

				current = current->m_Next;
			}

			std::cout << "\nMemory:\n";
			for (size_t i = 0; i < m_Capacity; ++i)
			{
				std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(m_Memory[i]) << " ";
			}
			std::cout << std::dec << std::endl;
		}
#endif

	private:

		void FindAndMergeFreeBlocks()
		{
			FreeListAllocatorNode* current = m_Head;

			while (current)
			{
				if (current->m_Next)
				{
					if (current->m_IsFree && current->m_Next->m_IsFree)
					{
						FreeListAllocatorNode* next_next = current->m_Next->m_Next;
						current->m_Size += current->m_Next->m_Size;
						delete current->m_Next;
						current->m_Next = next_next;
					}
				}

				current = current->m_Next;
			}
		
		}

	private:

		EMemoryCategory m_MemoryCategory;
		size_t m_Capacity;
		std::byte* m_Memory{ nullptr };
		FreeListAllocatorNode* m_Head{ nullptr };
	};
}