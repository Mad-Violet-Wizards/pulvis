#pragma once

#define ALLOCATOR_API(allocator_t, allocated_t) \
	public:																				\
	using value_type = allocated_t;							  \
	using pointer = allocated_t*;                 \
	using const_pointer = const allocated_t*;     \
	using reference = allocated_t&;				        \
	using const_reference = const allocated_t&;   \
	using size_type = std::size_t;				        \
	using difference_type = std::ptrdiff_t;       \
																														 \
	template<typename U> 																			 \
	struct rebind																							 \
	{																													 \
		using other = allocator_t<U>;		 										     \
	};																												 \
																														 \
	template<typename U, typename... Args>										 \
	void construct(U* _ptr, Args&&... _args) const noexcept	   \
	{																												   \
		new (_ptr) U(std::forward<Args>(_args)...);						   \
	}																												   \
																														 \
	template<typename U>																			 \
	void destroy(U* _ptr) const noexcept											 \
	{																												   \
		_ptr->~U();																							 \
	}																													 \
																														 \
	bool operator==(const allocator_t& _rhs) const noexcept    \
	{																													 \
		return true;																						 \
	}																													 \
																														 \
	bool operator!=(const allocator_t& _rhs) const noexcept    \
	{																													 \
		return !(*this == _rhs);																 \
	}																													 \
																														 \

