#pragma once

#include "PoolAllocator.hpp"

template<typename T, class Allocator>
class StlProxyAllocator
{
	public:

	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	StlProxyAllocator() = delete;

	explicit StlProxyAllocator(Allocator& _allocator) noexcept
		: m_Allocator(_allocator)
	{
	}

	template<typename U>
	StlProxyAllocator(const StlProxyAllocator<U, Allocator>& _other) noexcept
		: m_Allocator(_other.m_Allocator)
	{
	}

	[[nodiscard]] constexpr T* allocate(size_type _n)
	{
		return reinterpret_cast<T*>(m_Allocator.Alloc(_n));
	}

	void deallocate(T* _ptr, size_type _n) noexcept
	{
		m_Allocator.Dealloc(_ptr, _n);
	}

	template<typename U>
	struct rebind
	{
		using other = StlProxyAllocator<U, Allocator>;
	};

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

	bool operator==(const StlProxyAllocator& _rhs) const noexcept
	{
		return true;
	}

	bool operator!=(const StlProxyAllocator& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}

	Allocator& m_Allocator;
};