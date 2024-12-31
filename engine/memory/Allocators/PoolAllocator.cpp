#include "engine/engine_pch.hpp"
#include "PoolAllocator.hpp"

namespace engine::memory
{
	Bucket::Bucket(EMemoryCategory _mem_category, size_t _block_size, size_t _block_count)
	{
		m_MemoryCategory = _mem_category;
		m_BlockSize = _block_size;
		m_BlockCount = _block_count;

		const size_t data_size = m_BlockCount * m_BlockSize;
		m_Memory = static_cast<std::byte*>(std::malloc(data_size));

		const size_t ledger_size = m_BlockCount;
		m_Ledger = static_cast<std::byte*>(engine::memory::Allocate(_mem_category, ledger_size));

		std::memset(m_Ledger, 0, ledger_size);
		std::memset(m_Memory, 0, data_size);
	}

	Bucket::Bucket(const Bucket& _bucket)
	{
		m_MemoryCategory = _bucket.m_MemoryCategory;
		m_BlockSize = _bucket.m_BlockSize;
		m_BlockCount = _bucket.m_BlockCount;

		const size_t data_size = m_BlockCount * m_BlockSize;
		m_Memory = static_cast<std::byte*>(std::malloc(data_size));

		const size_t ledger_size = m_BlockCount;
		m_Ledger = static_cast<std::byte*>(std::malloc(ledger_size));

		std::memcpy(m_Ledger, _bucket.m_Ledger, ledger_size);
		std::memcpy(m_Memory, _bucket.m_Memory, data_size);
	}

	Bucket& Bucket::operator=(const Bucket& _bucket)
	{
		if (this != &_bucket)
		{
			if (m_Memory != nullptr)
			{
				engine::memory::Deallocate(m_MemoryCategory, m_Memory);
			}

			if (m_Ledger != nullptr)
			{
				engine::memory::Deallocate(m_MemoryCategory, m_Ledger);
			}

			m_MemoryCategory = _bucket.m_MemoryCategory;
			m_BlockSize = _bucket.m_BlockSize;
			m_BlockCount = _bucket.m_BlockCount;

			const size_t data_size = m_BlockCount * m_BlockSize;
			m_Memory = static_cast<std::byte*>(std::malloc(data_size));

			const size_t ledger_size = m_BlockCount;
			m_Ledger = static_cast<std::byte*>(std::malloc(ledger_size));

			std::memcpy(m_Ledger, _bucket.m_Ledger, ledger_size);
			std::memcpy(m_Memory, _bucket.m_Memory, data_size);
		}

		return *this;
	}

	Bucket::Bucket(Bucket&& _bucket)
		: m_MemoryCategory(_bucket.m_MemoryCategory)
		, m_BlockSize(_bucket.m_BlockSize)
		, m_BlockCount(_bucket.m_BlockCount)
		, m_Memory(std::move(_bucket.m_Memory))
		, m_Ledger(std::move(_bucket.m_Ledger))
	{
		_bucket.m_Memory = nullptr;
		_bucket.m_Ledger = nullptr;
	}

	Bucket& Bucket::operator=(Bucket&& _bucket)
	{
		if (this != &_bucket)
		{
			if (m_Memory != nullptr)
			{
				engine::memory::Deallocate(m_MemoryCategory, m_Memory);
			}

			if (m_Ledger != nullptr)
			{
				engine::memory::Deallocate(m_MemoryCategory, m_Ledger);
			}

			m_BlockSize = _bucket.m_BlockSize;
			m_BlockCount = _bucket.m_BlockCount;
			m_MemoryCategory = _bucket.m_MemoryCategory;

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
			engine::memory::Deallocate(m_MemoryCategory, m_Memory);
		}

		if (m_Ledger != nullptr)
		{
			engine::memory::Deallocate(m_MemoryCategory, m_Ledger);
		}
	}

	bool Bucket::Owns(void* ptr) const
	{
		return ptr >= m_Memory && ptr < m_Memory + m_BlockCount * m_BlockSize;
	}

	void* Bucket::Allocate(size_t _size)
	{
		const size_t blocks_needed = (_size + m_BlockSize - 1) / m_BlockSize;

		const size_t index = FindFreeBlocks(blocks_needed);
		if (index == -1)
		{
			return nullptr;
		}

		SetBlocksInUse(index, blocks_needed);
		return m_Memory + (index * m_BlockSize);
	}

	void Bucket::Deallocate(void* _ptr, size_t _size)
	{
		const std::byte* ptr = static_cast<const std::byte*>(_ptr);
		const size_t distance = static_cast<size_t>(ptr - m_Memory);
		const size_t index = distance / m_BlockSize;
		const size_t blocks_needed = (_size + m_BlockSize - 1) / m_BlockSize;
		SetBlocksFree(index, blocks_needed);
	}

	size_t Bucket::GetBlockSize() const
	{
		return m_BlockSize;
	}

	size_t Bucket::GetBlockCount() const
	{
		return m_BlockCount;
	}

	bool Bucket::IsFull() const
	{
		for (size_t i = 0; i < m_BlockCount; ++i)
		{
			if (static_cast<size_t>(m_Ledger[i]) == 0)
			{
				return false;
			}
		}

		return true;
	}

	size_t Bucket::FindFreeBlocks(size_t _size)
	{
		size_t free_blocks = 0;

		for (size_t i = 0; i < m_BlockCount; ++i)
		{
			if (static_cast<size_t>(m_Ledger[i]) == 0)
			{
				++free_blocks;

				if (free_blocks == _size)
				{
					return i - _size + 1;
				}
			}
			else
			{
				free_blocks = 0;
			}
		}

		return -1;
	}

	void Bucket::SetBlocksInUse(size_t _index, size_t _size)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			m_Ledger[_index + i] = static_cast<std::byte>(1);
		}
	}

	void Bucket::SetBlocksFree(size_t _index, size_t _size)
	{
		for (size_t i = 0; i < _size; ++i)
		{
			m_Ledger[_index + i] = static_cast<std::byte>(0);
		}
	}

}