#pragma once

#include "engine/memory/MemoryCategory.hpp"
#include "engine/memory/MemoryAlign.hpp"
#include "engine/memory/Allocators/StlProxyAllocator.hpp"
#include "engine/memory/Allocators/PoolAllocator.hpp"

namespace engine::pstd
{
	template<typename T, typename Allocator>
	using vector = std::vector<T, StlProxyAllocator<T, Allocator>>;
}