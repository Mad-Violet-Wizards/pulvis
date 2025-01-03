#include "engine/engine_pch.hpp"
#include "MemoryTests.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#include "engine/memory/Memory.hpp"
#include "engine/memory/MemoryProfiler.hpp"
#include "engine/memory/Allocators/FrameAllocator.hpp"
#include "engine/memory/Allocators/PoolAllocator.hpp"
#include "engine/memory/Allocators/FreeListAllocator.hpp"
#include "engine/containers/Vector.hpp"
TEST_CASE("Allocate and Deallocate", "[Memory]")
{
	using namespace engine::memory;
	using namespace engine::memory::tests;

	SECTION("One byte")
	{
		CMemoryProfiler::GetInstance().SetActive(true);
		OneByteClass* one_byte = Allocate<OneByteClass>(EMemoryCategory::Test);
		REQUIRE(one_byte != nullptr);
		REQUIRE(CMemoryProfiler::GetInstance().GetMemoryUsage(EMemoryCategory::Test) == sizeof(OneByteClass));
		Deallocate(EMemoryCategory::Test, one_byte);
		REQUIRE(CMemoryProfiler::GetInstance().GetMemoryUsage(EMemoryCategory::Test) == 0);
		CMemoryProfiler::GetInstance().SetActive(false);
	}

	SECTION("One kilobyte")
	{
		OneTwentyEightByteClass* one_kilobyte[8];
		CMemoryProfiler::GetInstance().SetActive(true);
		for (size_t i = 0; i < 8; ++i)
		{
			one_kilobyte[i] = Allocate<OneTwentyEightByteClass>(EMemoryCategory::Test);
			REQUIRE(one_kilobyte[i] != nullptr);
		}

		REQUIRE(CMemoryProfiler::GetInstance().GetMemoryUsage(EMemoryCategory::Test) == 1024);

		for (size_t i = 0; i < 8; ++i)
		{
			Deallocate(EMemoryCategory::Test, one_kilobyte[i]);
		}

		REQUIRE(CMemoryProfiler::GetInstance().GetMemoryUsage(EMemoryCategory::Test) == 0);
	}

	SECTION("One megabyte - allocated by array allocation")
	{
		CMemoryProfiler::GetInstance().SetActive(true);
		OneTwentyEightByteClass* one_megabyte = Allocate<OneTwentyEightByteClass>(EMemoryCategory::Array, 8192);
		REQUIRE(one_megabyte != nullptr);
		REQUIRE(CMemoryProfiler::GetInstance().GetMemoryUsage(EMemoryCategory::Array) == 1024 * 1024);
		Deallocate(EMemoryCategory::Array, one_megabyte, 8192);
		REQUIRE(CMemoryProfiler::GetInstance().GetMemoryUsage(EMemoryCategory::Array) == 0);
		CMemoryProfiler::GetInstance().SetActive(false);
	}
}

TEST_CASE("Frame Allocator", "[Memory]")
{
	using namespace engine::memory;
	using namespace engine::memory::tests;
//
//	engine::core::CAssertManager::GetInstance().SetActive(false);
//
//	SECTION("Allocate & run out of memory")
//	{
//		FrameAllocator<OneByteClass> frame_allocator(EMemoryCategory::Test, 1024);
//
//		for (size_t i = 0; i < 1024; ++i)
//		{
//			OneByteClass* one_byte = frame_allocator.Allocate(1);
//			REQUIRE(one_byte != nullptr);
//		}
//
//		REQUIRE(frame_allocator.Alloc(1) == nullptr);
//	}
//
//	SECTION("Std::array with frame allocator.")
//	{
//		FrameAllocator<OneByteClass> frame_allocator(EMemoryCategory::Test, 1024);
//		std::array<OneByteClass*, 1024> one_byte_array;
//
//		for (size_t i = 0; i < 1024; ++i)
//		{
//			one_byte_array[i] = frame_allocator.Allocate(1);
//			REQUIRE(one_byte_array[i] != nullptr);
//		}
//	}
	SECTION("Containers with frame allocator")
	{
		FrameAllocator<OneByteClass> frame_allocator(EMemoryCategory::Test, 512);
		Vector<OneByteClass, FrameAllocator<OneByteClass>> one_byte_vector(512, frame_allocator);

		for (size_t i = 0; i < 512; ++i)
		{
			one_byte_vector.PushBack(OneByteClass());
		}
	}
}

TEST_CASE("Pool allocator", "[Memory]")
{
	using namespace engine::memory;
	using namespace engine::memory::tests;

	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types with pool allocator")
	{
		PoolAllocator<int> pool_allocator(EMemoryCategory::Test, 1, 1, 16);

		int* int_ptr = engine::memory::New<int>(pool_allocator, 8);
		REQUIRE(int_ptr != nullptr);
		REQUIRE(int_ptr[0] == 8);

		int* int_ptr2 = engine::memory::New<int>(pool_allocator, 16);
		REQUIRE(int_ptr2 != nullptr);
		REQUIRE(int_ptr2[0] == 16);

		int* int_ptr3 = engine::memory::New<int>(pool_allocator, 32);
		REQUIRE(int_ptr3 != nullptr);
		REQUIRE(int_ptr3[0] == 32);

		int* int_ptr4 = engine::memory::New<int>(pool_allocator, 64);
		REQUIRE(int_ptr4 != nullptr);
		REQUIRE(int_ptr4[0] == 64);

		int* int_ptr5 = engine::memory::New<int>(pool_allocator, 128);
		REQUIRE(int_ptr5 == nullptr);

		pool_allocator.DumpConsole();
	}

	SECTION("Containers with pool allocator")
	{
		PoolAllocator<OneByteClass> pool_allocator(EMemoryCategory::Test, 1, 1, 512);
		Vector<OneByteClass, PoolAllocator<OneByteClass>> one_byte_vector(512, pool_allocator);

		for (size_t i = 0; i < 512; ++i)
		{
			one_byte_vector.PushBack(OneByteClass());
		}
	}
}

TEST_CASE("Free list allocator", "[Memory]")
{
	using namespace engine::memory;
	using namespace engine::memory::tests;

	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types with free list allocator")
	{
		FreeListAllocator<int> free_list_allocator(EMemoryCategory::Test, 16);

		int* int_ptr = engine::memory::New<int>(free_list_allocator, 16);
		REQUIRE(int_ptr != nullptr);
		REQUIRE(int_ptr[0] == 16);
	}

	SECTION("Containers with free list allocator")
	{
		FreeListAllocator<OneByteClass> free_list_allocator(EMemoryCategory::Test, 512);
		Vector<OneByteClass, FreeListAllocator<OneByteClass>> one_byte_vector(free_list_allocator);

		for (size_t i = 0; i < 256; ++i)
		{
			OneByteClass one_byte_class = OneByteClass(std::byte(i));
			one_byte_vector.PushBack(one_byte_class);
		}

		//free_list_allocator.DumpConsole();

		OneByteClass* one_byte = engine::memory::New<OneByteClass>(free_list_allocator, std::byte(64));

		//free_list_allocator.DumpConsole();
	}

	SECTION("Free list allocator with alignment")
	{
		FreeListAllocator<TwoByteClass> free_list_allocator(EMemoryCategory::Test, 512);
		Vector<TwoByteClass, FreeListAllocator<TwoByteClass>> two_byte_vector(free_list_allocator);

		for (size_t i = 0; i < 128; ++i)
		{
			TwoByteClass two_byte_class;
			two_byte_vector.PushBack(two_byte_class);
		}

		//free_list_allocator.DumpConsole();
	}
}