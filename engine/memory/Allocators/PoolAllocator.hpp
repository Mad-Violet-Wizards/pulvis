#pragma once

#include "engine/memory/MacroUtility.hpp"
#include "engine/memory/MemoryCategory.hpp"
#include "engine/rtti/RTTIEnum.hpp"

namespace engine::memory
{
	class PULVIS_API Bucket
	{

	public:

		Bucket(EMemoryCategory _mem_category, size_t _block_size, size_t _block_count);
		Bucket(const Bucket& _bucket);
		Bucket& operator=(const Bucket& _bucket);

		Bucket(Bucket&& _bucket);
		Bucket& operator=(Bucket&& _bucket);

		~Bucket();

		bool Owns(void* ptr) const;

		[[nodiscard]] void* Allocate(size_t _size);
		void Deallocate(void* _ptr, size_t _size);

		size_t GetBlockSize() const;
		size_t GetBlockCount() const;

		bool IsFull() const;

#ifdef DEBUG
		void DumpConsole();
#endif

	private:

		size_t FindFreeBlocks(size_t _size);
		void SetBlocksInUse(size_t _index, size_t _size);
		void SetBlocksFree(size_t _index, size_t _size);

	private:

		EMemoryCategory m_MemoryCategory;
		size_t m_BlockSize;
		size_t m_BlockCount;

		std::byte* m_Memory{ nullptr };
		std::byte* m_Ledger{ nullptr };
	};

	template<typename T>
	class PoolAllocator
	{
		ALLOCATOR_API(PoolAllocator, T);

	public:
		PoolAllocator(EMemoryCategory _mem_category, size_t _bucket_count, size_t _block_size, size_t _block_count)
			: m_MemoryCategory(_mem_category)
			, m_BucketCount(_bucket_count)
			, m_BlockCount(_block_count)
			, m_BlockSize(_block_size)
		{
			for (size_t i = 0; i < m_BucketCount; ++i)
			{
				m_Buckets.emplace_back(m_MemoryCategory, m_BlockSize, m_BlockCount);
			}
		}

		template<typename U>
		PoolAllocator(const PoolAllocator<U>& _other) noexcept
		{
			m_MemoryCategory = _other.GetMemoryCategory();
			m_BucketCount = _other.GetBucketCount();
			m_BlockSize = _other.GetBlockSize();
			m_BlockCount = _other.GetBlockCount();
			m_Buckets = _other.CopyBuckets();
		}

		template<typename U>
		PoolAllocator& operator=(const PoolAllocator<U>& _other) noexcept
		{
			if (this != &_other)
			{
				m_MemoryCategory = _other.GetMemoryCategory();
				m_BucketCount = _other.GetBucketCount();
				m_BlockSize = _other.GetBlockSize();
				m_BlockCount = _other.GetBlockCount();
				m_Buckets = _other.CopyBuckets();
			}

			return *this;
		}

		template<typename U>
		PoolAllocator(PoolAllocator<U>&& _other) noexcept
		{
			m_MemoryCategory = _other.GetMemoryCategory();
			m_BucketCount = _other.GetBucketCount();
			m_BlockSize = _other.GetBlockSize();
			m_BlockCount = _other.GetBlockCount();

			m_Buckets = _other.CopyBuckets();

			_other.m_MemoryCategory = EMemoryCategory::None;
			_other.m_BucketCount = 0;
			_other.m_BlockSize = 0;
			_other.m_BlockCount = 0;
			_other.m_Buckets.clear();
		}

		template<typename U>
		PoolAllocator& operator=(PoolAllocator<U>&& _other) noexcept
		{
			if (this != &_other)
			{
				m_MemoryCategory = _other.GetMemoryCategory();
				m_BucketCount = _other.GetBucketCount();
				m_BlockSize = _other.GetBlockSize();
				m_BlockCount = _other.GetBlockCount();

				m_Buckets = _other.CopyBuckets();

				_other.m_MemoryCategory = EMemoryCategory::None;
				_other.m_BucketCount = 0;
				_other.m_BlockSize = 0;
				_other.m_BlockCount = 0;
				_other.m_Buckets.clear();
			}

			return *this;
		}

		T* Allocate(size_t _size)
		{
			for (auto& bucket : m_Buckets)
			{
				if (!bucket.IsFull())
				{
					void* ptr = bucket.Allocate(_size);

					if (ptr != nullptr)
					{
						return static_cast<T*>(ptr);
					}
				}
			}

			return nullptr;
		}

		void Deallocate(T* _ptr, size_t _size)
		{
			for (auto& bucket : m_Buckets)
			{
				if (bucket.Owns(_ptr))
				{
					bucket.Deallocate(_ptr, _size);
					return;
				}
			}
		}

		EMemoryCategory GetMemoryCategory() const
		{
			return m_MemoryCategory;
		}

		size_t GetBucketCount() const
		{
			return m_BucketCount;
		}

		size_t GetBlockSize() const
		{
			return m_BlockSize;
		}

		size_t GetBlockCount() const
		{
			return m_BlockCount;
		}

		std::vector<Bucket> CopyBuckets() const
		{
			return m_Buckets;
		}

#ifdef DEBUG
		void DumpConsole()
		{
			std::cout << "[PoolAllocator] Dump\n";
			std::cout << "Memory Category: " << engine::rtti::CRTTIEnum<EMemoryCategory>::ToString(m_MemoryCategory) << "\n";
			std::cout << "Bucket Count: " << m_BucketCount << "\n";
			std::cout << "Block Size: " << m_BlockSize << "\n";
			std::cout << "Block Count: " << m_BlockCount << "\n";

			for (int i = 0; i < m_Buckets.size(); ++i)
			{
				std::cout << "\nBucket: " << i << "\n";
				m_Buckets[i].DumpConsole();
			}
		}
#endif

	private:

		EMemoryCategory m_MemoryCategory;
		size_t m_BucketCount;
		size_t m_BlockSize;
		size_t m_BlockCount;
		std::vector<Bucket> m_Buckets;
	};
}