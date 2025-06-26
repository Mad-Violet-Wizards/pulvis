#include "engine/engine_pch.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#include "engine/experimental/memory/Memory.hpp"
#include "engine/experimental/memory/MemoryProfiler.hpp"
#include "engine/experimental/memory/Allocators/FrameAllocator.hpp"
#include "engine/experimental/memory/Allocators/PoolAllocator.hpp"
#include "engine/experimental/memory/Allocators/FreeListAllocator.hpp"
#include "engine/experimental/memory/Allocators/SlabAllocator.hpp"

#if defined(BENCHMARKING)

using namespace engine::experimental::memory;
using namespace tests::experimental::memory;
using namespace engine::experimental::containers;

TEST_CASE("Allocators benchmark", "[Memory]")
{
	SECTION("Frame allocator benchmark")
	{
		static size_t FRAME_ALLOCATOR_BENCHMARK_ALLOCATIONS = 0;

		BENCHMARK("Frame allocator")
		{
			FrameAllocator<int> frame_allocator(EMemoryCategory::Test, ALLOCATOR_BENCHMARK_SIZE);

			for (size_t i = 0; i < BENCHMARK_ITERATIONS; ++i)
			{
				int* int_ptr = engine::experimental::memory::New<int>(frame_allocator);
				FRAME_ALLOCATOR_BENCHMARK_ALLOCATIONS++;
			}
		};

		std::cout << "\nFrame allocator benchmark allocations: " << FRAME_ALLOCATOR_BENCHMARK_ALLOCATIONS << "\n";
		std::cout << "Frame allocator allocated a total of: " << FRAME_ALLOCATOR_BENCHMARK_ALLOCATIONS * sizeof(int) << " bytes\n";
	}

    SECTION("Pool allocator benchmark")
	{
		static size_t POOL_ALLOCATOR_BENCHMARK_ALLOCATIONS = 0;

		BENCHMARK("Pool allocator")
		{
			PoolAllocator<int, 1> pool_allocator(EMemoryCategory::Test, ALLOCATOR_BENCHMARK_SIZE);

			for (size_t i = 0; i < BENCHMARK_ITERATIONS; ++i)
			{
				int* int_ptr = engine::experimental::memory::New<int>(pool_allocator, INT_MAX);
				POOL_ALLOCATOR_BENCHMARK_ALLOCATIONS++;
			}
		};

		std::cout << "\nPool allocator benchmark allocations: " << POOL_ALLOCATOR_BENCHMARK_ALLOCATIONS << "\n";
		std::cout << "Pool allocator allocated a total of: " << POOL_ALLOCATOR_BENCHMARK_ALLOCATIONS * sizeof(int) << " bytes\n";
	}

	SECTION("Free list allocator benchmark")
	{
		static size_t FREE_LIST_ALLOCATOR_BENCHMARK_ALLOCATIONS = 0;

		BENCHMARK("Free list allocator")
		{
			FreeListAllocator free_list_allocator(EMemoryCategory::Test, ALLOCATOR_BENCHMARK_SIZE);

			for (size_t i = 0; i < BENCHMARK_ITERATIONS; ++i)
			{
				int* int_ptr = engine::experimental::memory::New<int>(free_list_allocator, INT_MAX);
				FREE_LIST_ALLOCATOR_BENCHMARK_ALLOCATIONS++;
			}
		};

		std::cout << "\nFree list allocator benchmark allocations: " << FREE_LIST_ALLOCATOR_BENCHMARK_ALLOCATIONS << "\n";
		std::cout << "Free list allocator allocated a total of: " << FREE_LIST_ALLOCATOR_BENCHMARK_ALLOCATIONS * sizeof(int) << " bytes\n";
	}

	SECTION("Slab allocator benchmark")
	{
		SlabAllocator<2> slab_allocator(EMemoryCategory::Test);
		static size_t SLAB_ALLOCATOR_BENCHMARK_ALLOCATIONS = 0;

		BENCHMARK("Slab allocator")
		{
			slab_allocator.CreateSlab<int>(ALLOCATOR_BENCHMARK_SIZE);

			for (size_t i = 0; i < BENCHMARK_ITERATIONS; ++i)
			{
				int* int_ptr = engine::experimental::memory::New<int>(slab_allocator, INT_MAX);
				SLAB_ALLOCATOR_BENCHMARK_ALLOCATIONS++;
			}

			slab_allocator.Release();
		};

		std::cout << "\nSlab allocator benchmark allocations: " << SLAB_ALLOCATOR_BENCHMARK_ALLOCATIONS << "\n";
		std::cout << "Slab allocator allocated a total of: " << SLAB_ALLOCATOR_BENCHMARK_ALLOCATIONS * sizeof(int) << " bytes\n";
	}

    TEST_CASE("std::new / delete")
    {
        BENCHMARK("std::new / delete")
        {
            for (size_t i = 0; i < engine::experimental::memory::tests::BENCHMARK_ITERATIONS; ++i)
            {
                int* int_ptr = new int(INT_MAX);
                delete int_ptr;
            }
        };

        BENCHMARK("Shared pointers")
        {
            for (size_t i = 0; i < engine::experimental::memory::tests::BENCHMARK_ITERATIONS; ++i)
            {
                std::shared_ptr<int> int_ptr = std::make_unique<int>(INT_MAX);
            }
        };
    }
}


#endif // BENCHMARKING