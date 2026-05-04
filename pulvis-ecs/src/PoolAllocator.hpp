#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include <bit>
#include <new>
#include <type_traits>
#include <utility>

#include "Assert.hpp"

namespace pulvis::ecs
{
	/*
		Growth-sttable, page-allocated contiguous storage.

		INvariants:
		 - Pages, noce allocated, are never reallocated nor moved.
		 All pointers/references obtained via At()/SlotPtr() remain
		 valid until the slot is destroyed via PopBack()/Shrink()/Clear().
		 - Slots are SIMD-safe aligned (alignof(T))
		 - PageSize must be a power of two.

		 This class is intenteed to not be an ECS sparse-set.
		 It tracks no entity identity nor liveness map, no free list.
		 Use CComponentStorage on top of it.
	*/

	template<typename T, size_t PageSize = 1024>
	class CPoolAllocator final
	{
	public:

		static_assert(std::has_single_bit(PageSize), "PageSize must be a power of two.");

		using value_type = T;

		CPoolAllocator() noexcept = default;
		~CPoolAllocator() noexcept
		{
			DestroyAll();
			ReleasePages(0);
		}

		CPoolAllocator(const CPoolAllocator&) = delete;
		CPoolAllocator& operator=(const CPoolAllocator&) = delete;

		CPoolAllocator(CPoolAllocator&& _other) noexcept
			: m_Pages(std::move(_other.m_Pages))
			, m_Size(_other.m_Size)
		{
			_other.m_Size = 0;
		}

		CPoolAllocator& operator=(CPoolAllocator&& _other) noexcept
		{
			if (this != &_other)
			{
				DestroyAll();
				ReleasePages(0);
				m_Pages = std::move(_other.m_Pages);
				m_Size = _other.m_Size;
				_other.m_Size = 0;
			}

			return *this;
		}

		void Reserve(size_t _new_capacity)
		{
			EnsurePages((_new_capacity + PageSize - 1) >> PAGE_SHIFT);
		}

		template<typename... Args>
		T& EmplaceBack(Args&&... _args)
		{
			EnsurePages((m_Size >> PAGE_SHIFT) + 1);
			T* slot = SlotPtr(m_Size);
			::new (static_cast<void*>(slot)) T(std::forward<Args>(_args)...);
			++m_Size;
			return *slot;
		}

		void PopBack() noexcept(std::is_nothrow_destructible_v<T>)
		{
			ASSERT(m_Size > 0, "Cannot pop from an empty allocator.");
			--m_Size;
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				SlotPtr(m_Size)->~T();
			}
		}

		void Shrink(size_t _new_size) noexcept(std::is_nothrow_destructible_v<T>)
		{
			ASSERT(_new_size <= m_Size, "New size must be less than or equal to current size.");
			DestroyRange(_new_size, m_Size);
			m_Size = _new_size;
			ReleasePages((m_Size + PageSize - 1) >> PAGE_SHIFT);
		}

		void Clear() noexcept(std::is_nothrow_destructible_v<T>)
		{
			DestroyAll();
			m_Size = 0;
		}

		void ShrinkToFit()
		{
			ReleasePages((m_Size + PageSize - 1) >> PAGE_SHIFT);
		}

		[[nodiscard]] T& At(size_t _index) noexcept
		{
			ASSERT(_index < m_Size, "Index out of bounds.");
			return *SlotPtr(_index);
		}

		[[nodiscard]] const T& At(size_t _index) const noexcept
		{
			ASSERT(_index < m_Size, "Index out of bounds.");
			return *SlotPtr(_index);
		}

		[[nodiscard]] T& operator[](size_t _index) noexcept { return At(_index); }
		[[nodiscard]] const T& operator[](size_t _index) const noexcept { return At(_index); }

		[[nodiscard]] size_t Size() const noexcept { return m_Size; }
		[[nodiscard]] bool   Empty() const noexcept { return m_Size == 0; }
		[[nodiscard]] size_t Capacity() const noexcept { return m_Pages.size() * PageSize; }
		[[nodiscard]] size_t PageCount() const noexcept { return m_Pages.size(); }
		[[nodiscard]] size_t MemoryFootprint() const noexcept { return m_Pages.size() * (sizeof(T) * PageSize); }

		[[nodiscard]] static constexpr size_t PageCapacity() noexcept { return PageSize; }

		template<typename Fn>
		void ForEach(Fn&& _fn)
		{
			ForEachImpl(*this, _fn);
		}

		template<typename Fn>
		void ForEach(Fn&& _fn) const
		{
			ForEachImpl(*this, _fn);
		}

	private:

		static constexpr size_t PAGE_SHIFT = static_cast<size_t>(std::countr_zero(PageSize));
		static constexpr size_t PAGE_MASK = PageSize - 1;

		[[nodiscard]] T* SlotPtr(size_t _index) noexcept
		{
			return m_Pages[_index >> PAGE_SHIFT] + (_index & PAGE_MASK);
		}

		[[nodiscard]] const T* SlotPtr(size_t _index) const noexcept
		{
			return m_Pages[_index >> PAGE_SHIFT] + (_index & PAGE_MASK);
		}

		void EnsurePages(size_t _required_pages)
		{
			while (m_Pages.size() < _required_pages)
			{
				m_Pages.emplace_back(AllocatePage());
			}
		}

		void ReleasePages(size_t _kept_pages) noexcept
		{
			while (m_Pages.size() > _kept_pages)
			{
				DeallocatePage(m_Pages.back());
				m_Pages.pop_back();
			}
		}

		void DestroyRange(size_t _from, size_t _to) noexcept(std::is_nothrow_destructible_v<T>)
		{
			if constexpr (std::is_trivially_destructible_v<T>)
			{
				return;
			}

			for (size_t i = _from; i < _to; ++i)
			{
				SlotPtr(i)->~T();
			}
		}

		void DestroyAll() noexcept(std::is_nothrow_destructible_v<T>)
		{
			DestroyRange(0, m_Size);
		}

		[[nodiscard]] static T* AllocatePage()
		{
			constexpr size_t alignment = alignof(T);
			constexpr size_t bytes = sizeof(T) * PageSize;
			void* memory = ::operator new(bytes, std::align_val_t{ alignment });
			return static_cast<T*>(memory);
		}

		static void DeallocatePage(T* _page) noexcept
		{
			constexpr size_t alignment = alignof(T);
			::operator delete(static_cast<void*>(_page), std::align_val_t{ alignment });
		}

		template<typename U, typename Fn>
		static void ForEachImpl(U& _obj, Fn& _fn)
		{
			if (_obj.m_Size == 0)
			{
				return;
			}

			const size_t full_pages = _obj.m_Size >> PAGE_SHIFT;
			for (size_t page = 0; page < full_pages; ++page)
			{
				auto* page = _obj.m_Pages[page];
				for (size_t i = 0; i < PageSize; ++i)
				{
					_fn(page[i]);
				}

				const size_t remaining = _obj.m_Size & PAGE_MASK;
				if (remaining > 0)
				{
					page = _obj.m_Pages[full_pages];
					for (size_t i = 0; i < remaining; ++i)
					{
						_fn(page[i]);
					}
				}
			}
		}

	private:

		std::vector<T*> m_Pages;
		size_t m_Size = 0;
	};
}