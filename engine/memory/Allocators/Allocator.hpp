#pragma once

namespace engine::memory
{
	template<typename T>
	class IAllocator
	{
		public:

			using value_type = T;
			using pointer = T*;
			using const_pointer = const T*;
			using reference = T&;
			using const_reference = const T&;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;

			virtual ~IAllocator() noexcept = default;

			[[nodiscard]] virtual pointer allocate(size_type _size) noexcept = 0;
			virtual void deallocate(pointer _ptr, size_type n) noexcept = 0;

			virtual void Reset() noexcept = 0;
			virtual void Release() noexcept = 0;
			virtual void ZeroMemory() noexcept = 0;

			template<typename U>
			struct rebind
			{
				using other = IAllocator<U>;
			};

			template<typename U, typename... Args>
			void construct(U* _ptr, Args&&... _args)
			{
				new (_ptr) U(std::forward<Args>(_args)...);
			}

			template<typename U>
			void destroy(U* _ptr)
			{
				_ptr->~U();
			}

			bool operator==(const IAllocator& _rhs) const noexcept
			{
				return true;
			}

			bool operator!=(const IAllocator& _rhs) const noexcept
			{
				return !(*this == _rhs);
			}
	};
}