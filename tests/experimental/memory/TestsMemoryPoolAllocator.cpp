#include "engine/engine_pch.hpp"
#include "TestsMemoryMock.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#include "engine/experimental/memory/Memory.hpp"
#include "engine/experimental/memory/MemoryProfiler.hpp"
#include "engine/experimental/memory/Allocators/PoolAllocator.hpp"

#include "engine/experimental/containers/Vector.hpp"

using namespace engine::experimental::memory;
using namespace tests::experimental::memory;
using namespace engine::experimental::containers;

TEST_CASE("Pool allocator", "[Memory]")
{
	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types allocation with pool allocator")
	{
		PoolAllocator<int, 2> pool_allocator(EMemoryCategory::Test, 1024);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(pool_allocator, 16);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == 16);
		}
	}
	
	SECTION("Allocations and deallocations")
	{
		PoolAllocator<int, 2> pool_allocator(EMemoryCategory::Test, 1024);

		constexpr int first_loop_cstr_arg = 587'485;
		constexpr int second_loop_cstr_arg = 922'321;
		constexpr int third_loop_cstr_arg = 184'882;

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(pool_allocator, first_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == first_loop_cstr_arg);
		}

		pool_allocator.Reset();
		std::vector<int*> ptrs;
		size_t predicted_free_indexes = 0;
		std::vector<int> where;

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(pool_allocator, second_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == second_loop_cstr_arg);

			if (i % 3 == 0 || i % 5 == 0)
			{
				predicted_free_indexes++;
				where.push_back(i);
				ptrs.push_back(int_ptr);
			}
		}
			
		for (size_t i = 0; i < ptrs.size(); ++i)
		{
			engine::experimental::memory::Delete(pool_allocator, ptrs[i]);
		}

		for (size_t i = 0; i < predicted_free_indexes; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(pool_allocator, third_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == third_loop_cstr_arg);
		}
	}

	SECTION("Irregular types")
	{
		PoolAllocator<ThreeByteClass, 2> pool_allocator_three_bytes(EMemoryCategory::Test, 512);
		PoolAllocator<FourtyTwoByteClass, 2> pool_allocators_fourty_two_bytes(EMemoryCategory::Test, 512);

		for (size_t i = 0; i < 128; ++i)
		{
			ThreeByteClass* three_byte_class = engine::experimental::memory::New<ThreeByteClass>(pool_allocator_three_bytes);
			REQUIRE(three_byte_class != nullptr);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			FourtyTwoByteClass* fourty_two_byte_class = engine::experimental::memory::New<FourtyTwoByteClass>(pool_allocators_fourty_two_bytes);
			REQUIRE(fourty_two_byte_class != nullptr);
		}
	}

	SECTION("Containers with pool allocator")
	{
		PoolAllocator<OneByteClass, 2> pool_allocator(EMemoryCategory::Test, 512);
		Vector<OneByteClass, PoolAllocator<OneByteClass, 2>> one_byte_vector(512, pool_allocator);

		for (size_t i = 0; i < 512; ++i)
		{
			one_byte_vector.PushBack(OneByteClass());
		}
	}
}