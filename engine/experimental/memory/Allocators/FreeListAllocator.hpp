#pragma once

#include <iomanip>

#include "engine/experimental/memory/Memory.hpp"
#include "engine/experimental/memory/MemoryCategory.hpp"

namespace engine::experimental::memory
{
	class FreeListAllocatorNode
	{
	public:

		FreeListAllocatorNode* m_Next{ nullptr };
		size_t m_Size{ 0 };
		size_t m_Offset{ 0 };
		bool m_IsFree{ true };
	};

	class FreeListAllocator
	{
	public:

		FreeListAllocator(EMemoryCategory _mem_category, size_t _capacity)
			: m_MemoryCategory(_mem_category)
			, m_Capacity(_capacity)
		{
			m_Head = new FreeListAllocatorNode();
			m_Head->m_Size = _capacity;
			m_Head->m_Offset = 0;
			m_Memory = engine::experimental::memory::Allocate<std::byte>(_mem_category, _capacity);
			std::memset(m_Memory, 0, _capacity);
		}

		FreeListAllocator(const FreeListAllocator& _other) = delete;
		FreeListAllocator& operator=(const FreeListAllocator _other) = delete;

		FreeListAllocator(FreeListAllocator&& _other) noexcept
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

		FreeListAllocator& operator=(FreeListAllocator&& _other) noexcept
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
			engine::experimental::memory::Deallocate(m_MemoryCategory, m_Memory);

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

		template<typename T>
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

		template<typename T>
		void Deallocate(T* _ptr, size_t _size)
		{
			FreeListAllocatorNode* current = m_Head;
			FreeListAllocatorNode* previous = nullptr;

			while (current)
			{
				if (current->m_Size == _size && m_Memory + current->m_Offset == reinterpret_cast<std::byte*>(_ptr))
				{
					current->m_IsFree = true;
					break;
				}

				previous = current;
				current = current->m_Next;
			}

			FindAndMergeFreeBlocks();
		}

		void Reset()
		{
			FreeListAllocatorNode* head = m_Head;
			FreeListAllocatorNode* next = m_Head->m_Next;

			while (next)
			{
				head->m_Size += next->m_Size;
				FreeListAllocatorNode* temp = next;
				next = next->m_Next;
				delete temp;
			}

			m_Head->m_IsFree = true;
			m_Head->m_Next = nullptr;
			m_Head->m_Offset = 0;
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

		bool operator==(const FreeListAllocator& _rhs) const noexcept
		{
			return true;
		}

		bool operator!=(const FreeListAllocator& _rhs) const noexcept
		{
			return !(*this == _rhs);
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