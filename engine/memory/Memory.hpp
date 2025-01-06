#pragma once

#include "engine/core/Export.hpp"

namespace engine::memory
{
	enum class EMemoryCategory;

	PULVIS_API void* Allocate(EMemoryCategory _mem_category, size_t _size);
	PULVIS_API void* Reallocate(void* _ptr, size_t _size);
	PULVIS_API void Deallocate(EMemoryCategory _mem_category, void* _ptr);
	std::vector<std::string> GetStacktrace();

	template<typename T, class Alloc, typename... Args>
	T* New(Alloc& _alloc, Args&&... _args)
	{
		T* ptr = _alloc.Allocate<T>(sizeof(T));
		_alloc.construct(ptr, std::forward<Args>(_args)...);
		return ptr;
	}

	template<typename T, class Alloc>
	void Delete(Alloc& _alloc, T* _ptr)
	{
		_alloc.destroy(_ptr);
		_alloc.Deallocate(_ptr, sizeof(T));
	}

	template<typename T>
	T* Allocate(EMemoryCategory _mem_category, size_t count)
	{
		void* mem = Allocate(_mem_category, count);
		T* arr = static_cast<T*>(mem);

		for (size_t i = 0; i < count; ++i)
		{
			new (&arr[i]) T();
		}

		return arr;
	}

	template<typename T>
	void Deallocate(EMemoryCategory _mem_category, T* _ptr, size_t _count)
	{
		for (size_t i = 0; i < _count; ++i)
		{
			_ptr[i].~T();
		}

		Deallocate(_mem_category, static_cast<void*>(_ptr));
	}
}