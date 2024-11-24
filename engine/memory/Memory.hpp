#pragma once

#include "engine/core/Export.hpp"

namespace engine::memory
{
	enum class EMemoryCategory;

	template<typename T>
	T* Allocate(EMemoryCategory _mem_category)
	{
		return static_cast<T*>(Allocate(_mem_category, sizeof(T)));
	}

	template<typename T>
	void Deallocate(EMemoryCategory _mem_category, T* _ptr)
	{
		Deallocate(_mem_category, static_cast<void*>(_ptr));
	}

	template<typename T>
	T* Allocate(EMemoryCategory _mem_category, size_t count)
	{
		void* mem = Allocate(_mem_category, sizeof(T) * count);
		T* arr = static_cast<T*>(mem);

		for (size_t i = 0; i < count; ++i)
		{
			new (&arr[i]) T();
		}

		return arr;
	}

	template<typename T>
	void Deallocate(EMemoryCategory _mem_category, T* _ptr, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			_ptr[i].~T();
		}

		Deallocate(_mem_category, static_cast<void*>(_ptr));
	}

	PULVIS_API void* Allocate(EMemoryCategory _mem_category, size_t _size);
	PULVIS_API void Deallocate(EMemoryCategory _mem_category, void* _ptr);
	std::vector<std::string> GetStacktrace();
}