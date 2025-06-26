#include "engine/engine_pch.hpp"
#include "PoolAllocator.hpp"
#include <iomanip>

namespace engine::experimental::memory
{
	Bucket::Bucket()
		: m_MemoryCategory(EMemoryCategory::Memory)
		, m_Capacity(0)
		, m_TypeSize(0)
		, m_Size(0)
		, m_Memory(nullptr)
		, m_Ledger(nullptr)
	{
	}

	Bucket::Bucket(Bucket&& _bucket) noexcept
		: m_MemoryCategory(_bucket.m_MemoryCategory)
		, m_Capacity(_bucket.m_Capacity)
		, m_TypeSize(_bucket.m_TypeSize)
		, m_Size(_bucket.m_Size)
		, m_Memory(std::move(_bucket.m_Memory))
		, m_Ledger(std::move(_bucket.m_Ledger))
	{
		_bucket.m_Memory = nullptr;
		_bucket.m_Ledger = nullptr;
	}

	Bucket& Bucket::operator=(Bucket&& _bucket) noexcept
	{
		if (this != &_bucket)
		{
			m_MemoryCategory = _bucket.m_MemoryCategory;
			m_Capacity = _bucket.m_Capacity;
			m_TypeSize = _bucket.m_TypeSize;
			m_Size = _bucket.m_Size;
			m_Memory = std::move(_bucket.m_Memory);
			m_Ledger = std::move(_bucket.m_Ledger);

			_bucket.m_Memory = nullptr;
			_bucket.m_Ledger = nullptr;
		}

		return *this;
	}

	Bucket::~Bucket()
	{
		if (m_Memory != nullptr)
		{
			engine::experimental::memory::Deallocate(m_MemoryCategory, m_Memory);
		}

		if (m_Ledger != nullptr)
		{
			engine::experimental::memory::Deallocate(m_MemoryCategory, m_Ledger);
		}
	}

	bool Bucket::Owns(void* ptr) const
	{
		return ptr >= m_Memory && ptr < m_Memory + m_Capacity;
	}

	bool Bucket::IsFull() const
	{
		return m_Size >= m_Capacity;
	}

	void Bucket::DumpConsole()
	{
		std::cout << "Capacity: " << m_Capacity << "\n";
		std::cout << "Memory:\n";
		for (size_t i = 0; i < m_Capacity; ++i)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(m_Memory[i]) << " ";
		}
		std::cout << std::dec << std::endl;
	}

	size_t Bucket::FindFreeBlocks(size_t _size) const
	{
		size_t free_blocks = 0;
		const size_t ledger_size = m_Capacity / m_TypeSize;
		size_t index = 0;

		for (size_t i = 0; i < ledger_size; ++i)
		{
			if (static_cast<size_t>(m_Ledger[i]) == 0)
			{
				free_blocks += m_TypeSize;

				if (free_blocks == _size)
				{
					return index;
				}
			}
			else
			{
				index = i + 1;
				free_blocks = 0;
			}
		}

		return -1;
	}

	void Bucket::SetBlocksInUse(size_t _index, size_t _size)
	{
		std::memset(m_Ledger + _index, 1, _size / m_TypeSize);
	}

	void Bucket::SetBlocksFree(size_t _index, size_t _size)
	{
		std::memset(m_Ledger + _index, 0, _size / m_TypeSize);
	}

}