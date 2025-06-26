#include "engine/engine_pch.hpp"
#include "TestsMemoryMock.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#include "engine/experimental/memory/Memory.hpp"
#include "engine/experimental/memory/MemoryProfiler.hpp"
#include "engine/experimental/memory/Allocators/FreeListAllocator.hpp"

#include "engine/experimental/containers/Vector.hpp"

using namespace engine::experimental::memory;
using namespace tests::experimental::memory;
using namespace engine::experimental::containers;

TEST_CASE("Free list allocator", "[Memory]")
{
	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types with free list allocator")
	{
		FreeListAllocator free_list_allocator(EMemoryCategory::Test, 2048);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(free_list_allocator, 16);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == 16);
		}
	}

	SECTION("Allocations and deallocations")
	{
		constexpr int first_loop_cstr_arg = 742'111;
		constexpr int second_loop_cstr_arg = 384'127;
		constexpr int third_loop_cstr_arg = 217'283;

		FreeListAllocator free_list_allocator(EMemoryCategory::Test, 2048);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(free_list_allocator, first_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == first_loop_cstr_arg);
		}

		free_list_allocator.Reset();
		std::vector<int*> ptrs;

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(free_list_allocator, second_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == second_loop_cstr_arg);
			ptrs.push_back(int_ptr);
		}

		for (size_t i = 0; i < ptrs.size(); ++i)
		{
			engine::experimental::memory::Delete(free_list_allocator, ptrs[i]);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(free_list_allocator, third_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == third_loop_cstr_arg);
		}
	}

	SECTION("Irregular types")
	{
		FreeListAllocator free_list_allocator(EMemoryCategory::Test, 1024);

		for (size_t i = 0; i < 128; ++i)
		{
			ThreeByteClass* three_byte_class = engine::experimental::memory::New<ThreeByteClass>(free_list_allocator);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			FourtyTwoByteClass* fourty_two_byte_class = engine::experimental::memory::New<FourtyTwoByteClass>(free_list_allocator);
		}
	}

	SECTION("Containers with free list allocator")
	{
		FreeListAllocator free_list_allocator(EMemoryCategory::Test, 512);
		Vector<OneByteClass, FreeListAllocator> one_byte_vector(free_list_allocator);

		for (size_t i = 0; i < 256; ++i)
		{
			OneByteClass one_byte_class = OneByteClass(std::byte(i));
			one_byte_vector.PushBack(one_byte_class);
		}
	}
}