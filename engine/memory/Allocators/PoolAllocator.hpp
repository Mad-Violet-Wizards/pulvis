#pragma once

#include "engine/memory/MacroUtility.hpp"
#include "engine/memory/MemoryCategory.hpp"
#include "engine/rtti/RTTIEnum.hpp"

namespace engine::memory
{
	class PULVIS_API Bucket
	{

	public:

		Bucket();
		Bucket(const Bucket& _bucket);
		Bucket& operator=(const Bucket& _bucket);

		Bucket(Bucket&& _bucket) noexcept;
		Bucket& operator=(Bucket&& _bucket) noexcept;

		~Bucket();

		bool Owns(void* ptr) const;

		template<typename T>
		[[nodiscard]] T* Allocate(size_t _size)
		{
			const size_t free_block_index = FindFreeBlocks(_size);
			const size_t memory_index = free_block_index * m_TypeSize;

			T* ptr = reinterpret_cast<T*>(m_Memory + memory_index);
			SetBlocksInUse(free_block_index, _size);
			m_Size += _size;
			return ptr;
		}

		template<typename T>
		void Deallocate(T* _ptr, size_t _size)
		{
			const size_t memory_index = reinterpret_cast<std::byte*>(_ptr) - m_Memory;
			const size_t ledger_index = memory_index / m_TypeSize;

			SetBlocksFree(ledger_index, _size);
			m_Size -= _size;
		}

		bool IsFull() const;

#ifdef DEBUG
		void DumpConsole();
#endif

	private:

		size_t FindFreeBlocks(size_t _size) const;
		void SetBlocksInUse(size_t _index, size_t _size);
		void SetBlocksFree(size_t _index, size_t _size);

	public:

		EMemoryCategory m_MemoryCategory;
		size_t m_Capacity;
		size_t m_TypeSize;
		size_t m_Size;
		std::byte* m_Memory{ nullptr };
		std::byte* m_Ledger{ nullptr };
	};

	template<typename T, size_t N>
	class PoolAllocator
	{
	public:
		PoolAllocator(EMemoryCategory _mem_category, size_t _bucket_capacity)
			: m_MemoryCategory(_mem_category)
		{
			for (size_t i = 0; i < N; ++i)
			{
				m_Buckets[i].m_MemoryCategory = _mem_category;
				m_Buckets[i].m_Capacity = _bucket_capacity;
				m_Buckets[i].m_TypeSize = sizeof(T);
				m_Buckets[i].m_Memory = engine::memory::Allocate<std::byte>(_mem_category, _bucket_capacity);
				m_Buckets[i].m_Ledger = engine::memory::Allocate<std::byte>(_mem_category, _bucket_capacity / sizeof(T));
				std::memset(m_Buckets[i].m_Ledger, 0, _bucket_capacity / sizeof(T));
			}
		}

		template<typename U, size_t N>
		PoolAllocator(const PoolAllocator<U, N>& _other) noexcept
		{
			m_MemoryCategory = _other.GetMemoryCategory();
			m_Buckets = _other.CopyBuckets();
		}

		template<typename U, size_t N>
		PoolAllocator& operator=(const PoolAllocator<U, N>& _other) noexcept
		{
			if (this != &_other)
			{
				m_MemoryCategory = _other.GetMemoryCategory();
				m_Buckets = _other.CopyBuckets();
			}

			return *this;
		}

		template<typename U, size_t N>
		PoolAllocator(PoolAllocator<U, N>&& _other) noexcept
		{
			m_MemoryCategory = _other.GetMemoryCategory();
			m_Buckets = _other.CopyBuckets();

			_other.m_MemoryCategory = EMemoryCategory::None;
			_other.m_Buckets.clear();
		}

		template<typename U, size_t N>
		PoolAllocator& operator=(PoolAllocator<U, N>&& _other) noexcept
		{
			if (this != &_other)
			{
				m_MemoryCategory = _other.GetMemoryCategory();
				m_Buckets = _other.CopyBuckets();

				_other.m_MemoryCategory = EMemoryCategory::None;;
				_other.m_Buckets.clear();
			}

			return *this;
		}

		template<typename U>
		U* Allocate(size_t _size)
		{
			static_assert(std::is_same_v<U, T>, "PoolAllocator: U must be the same type as T!");

			for (Bucket& bucket : m_Buckets)
			{
				if (!bucket.IsFull())
				{
					return bucket.Allocate<T>(_size);
				}
			}

			return nullptr;
		}

		void Deallocate(T* _ptr, size_t _size)
		{
			for (Bucket& bucket : m_Buckets)
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

		std::array<Bucket, N> CopyBuckets() const
		{
			return m_Buckets;
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

		template<typename U, size_t N>
		bool operator==(const PoolAllocator<U, N>& _rhs) const noexcept
		{
			return true;
		}

		template<typename U, size_t N>
		bool operator!=(const PoolAllocator<U, N>& _rhs) const noexcept
		{
			return !(*this == _rhs);
		}

#ifdef DEBUG
		void DumpConsole()
		{
			std::cout << "[PoolAllocator] Dump\n";
			std::cout << "Memory Category: " << engine::rtti::CRTTIEnum<EMemoryCategory>::ToString(m_MemoryCategory) << "\n";
			std::cout << "Buckets:\n";

			for (int i = 0; i < m_Buckets.size(); ++i)
			{
				std::cout << "\nBucket: " << i << "\n";
				m_Buckets[i].DumpConsole();
			}
		}
#endif

	private:

		EMemoryCategory m_MemoryCategory;
		std::array<Bucket, N> m_Buckets;
	};
}