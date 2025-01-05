#include "engine/memory/MacroUtility.hpp"
#include "engine/memory/MemoryCategory.hpp"
#include <iomanip>

namespace engine::memory
{
	class SlabAllocatorNode
	{
		public:

			SlabAllocatorNode()
				: m_TypeHash(-1)
				, m_TypeSize(-1)
				, m_Capacity(0)
				, m_Offset(0)
				, m_Memory{ nullptr }
				, m_Ledger{ nullptr }
			{
			}

			~SlabAllocatorNode()
			{
				engine::memory::Deallocate(EMemoryCategory::Memory, m_Memory);
				engine::memory::Deallocate(EMemoryCategory::Memory, m_Ledger);
			}

			template<typename T>
			T* Allocate(size_t _size)
			{
				const size_t free_block_index = FindFreeBlocks(_size);
				const size_t memory_index = m_Offset + (free_block_index * m_TypeSize);

				T* ptr = reinterpret_cast<T*>(m_Memory + memory_index);
				SetBlocksInUse(free_block_index, _size);
				return ptr;
			}

			template<typename T>
			void Deallocate(T* _ptr, size_t _size)
			{
				const size_t memory_index = reinterpret_cast<std::byte*>(_ptr) - m_Memory;
				const size_t ledger_index = memory_index / m_TypeSize;

				SetBlocksFree(ledger_index, _size);
			}

#ifdef DEBUG
			void DumpConsole() const
			{
				std::cout << "\n[Memory]:\n";
				for (size_t i = 0; i < m_Capacity; i++)
				{
					std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(m_Memory[i]) << " ";
				}
				std::cout << std::dec << std::endl;
			}
#endif

		private:

			size_t FindFreeBlocks(size_t _size) const
			{
				size_t free_blocks = 0;
				const size_t ledger_size = m_Capacity / m_TypeSize;

				for (size_t i = 0; i < ledger_size; ++i)
				{
					if (static_cast<size_t>(m_Ledger[i]) == 0)
					{
						free_blocks += m_TypeSize;

						if (free_blocks == _size)
						{
							return i;
						}
					}
					else
					{
						free_blocks = 0;
					}
				}

				return -1;
			}

			void SetBlocksInUse(size_t _index, size_t _size)
			{
				std::memset(m_Ledger + _index, 1, _size / m_TypeSize);
			}

			void SetBlocksFree(size_t _index, size_t _size)
			{
				std::memset(m_Ledger + _index, 0, _size / m_TypeSize);
			}

		public:

			size_t m_TypeHash;
			size_t m_TypeSize;
			size_t m_Capacity;
			size_t m_Offset;
			std::byte* m_Memory;
			std::byte* m_Ledger;
	};

	template<size_t N>
	class SlabAllocator
	{
		public:
		
			SlabAllocator(EMemoryCategory _mem_category)
				: m_MemoryCategory(_mem_category)
			{
			}

			~SlabAllocator()
			{
			}

			template<typename T>
			void CreateSlab(size_t _capacity)
			{
				const size_t type_hash = std::type_index(typeid(T)).hash_code();

				for (SlabAllocatorNode& slab : m_Slabs)
				{
					if (slab.m_TypeHash == type_hash)
					{
						return;
					}

					if (slab.m_Capacity == 0)
					{
						slab.m_Capacity = _capacity;
						slab.m_TypeHash = type_hash;
						slab.m_TypeSize = sizeof(T);
						slab.m_Offset = 0;
						slab.m_Memory = engine::memory::Allocate<std::byte>(EMemoryCategory::Memory, slab.m_Capacity);
						slab.m_Ledger = engine::memory::Allocate<std::byte>(EMemoryCategory::Memory, slab.m_Capacity / slab.m_TypeSize);
						std::memset(slab.m_Ledger, 0, slab.m_Capacity / slab.m_TypeSize);
						return;
					}
				}
			}

			template<typename T>
			T* Allocate(size_t _size)
			{
				const size_t type_hash = std::type_index(typeid(T)).hash_code();

				for (SlabAllocatorNode& slab : m_Slabs)
				{
					if (slab.m_TypeHash == type_hash)
					{
						return slab.Allocate<T>(_size);
					}
				}

				return nullptr;
			}

			template<typename T>
			void Deallocate(T* _ptr, size_t _size)
			{
				const size_t type_hash = std::type_index(typeid(T)).hash_code();

				for (SlabAllocatorNode& slab : m_Slabs)
				{
					if (slab.m_TypeHash == type_hash)
					{
						slab.Deallocate<T>(_ptr, _size);
						return;
					}
				}
			}

			void Release()
			{
				for (SlabAllocatorNode& slab : m_Slabs)
				{
					std::memset(slab.m_Ledger, 0, slab.m_Capacity / slab.m_TypeSize);
					slab.m_Offset = 0;
				}
			}

#ifdef DEBUG
			void DumpConsole() const
			{
				std::cout << "[SlabAllocator] Dumping memory: \n";
				std::cout << "Memory Category: " << static_cast<int>(m_MemoryCategory) << "\n";
				std::cout << "Slabs:\n";

				for (const auto& slab : m_Slabs)
				{
					slab.DumpConsole();
				}
			}
			
#endif

		public:

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

			bool operator==(const SlabAllocator& _rhs) const noexcept
			{
				return true;
			}

			bool operator!=(const SlabAllocator& _rhs) const noexcept
			{
				return !(*this == _rhs);
			}

	private:

		EMemoryCategory m_MemoryCategory;
		std::array<SlabAllocatorNode, N> m_Slabs;
	};
}