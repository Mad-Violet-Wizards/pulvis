#include "engine/engine_pch.hpp"
#include "MemoryTests.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#include "engine/memory/Memory.hpp"
#include "engine/memory/MemoryProfiler.hpp"
#include "engine/memory/Allocators/FrameAllocator.hpp"
#include "engine/memory/Allocators/PoolAllocator.hpp"
#include "engine/memory/Allocators/FreeListAllocator.hpp"
#include "engine/memory/Allocators/SlabAllocator.hpp"

#include "engine/containers/Vector.hpp"

#define BENCHMARKING

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

	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types with frame alloactor")
	{
		FrameAllocator<int> frame_allocator(EMemoryCategory::Test, 2048);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(frame_allocator);
			REQUIRE(int_ptr != nullptr);
		}
	}

	SECTION("Containers with frame allocator")
	{
		FrameAllocator<OneByteClass> frame_allocator(EMemoryCategory::Test, 512);
		Vector<OneByteClass, FrameAllocator<OneByteClass>> one_byte_vector(512, frame_allocator);

		for (size_t i = 0; i < 512; ++i)
		{
			one_byte_vector.PushBack(OneByteClass());
		}
	}

#if defined(BENCHMARKING)
	SECTION("Frame allocator benchmark (vs std's new)")
	{
		BENCHMARK("Frame allocator")
		{
			FrameAllocator<int> frame_allocator(EMemoryCategory::Test, 2048);

			for (size_t i = 0; i < 512; ++i)
			{
				int* int_ptr = engine::memory::New<int>(frame_allocator);
			}
		};
	}
#endif
}

TEST_CASE("Pool allocator", "[Memory]")
{
	using namespace engine::memory;
	using namespace engine::memory::tests;

	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types with pool allocator")
	{
		//PoolAllocator<int> pool_allocator(EMemoryCategory::Test, 1, 1, 16);

		//int* int_ptr = engine::memory::New<int>(pool_allocator, 8);
		//REQUIRE(int_ptr != nullptr);
		//REQUIRE(int_ptr[0] == 8);

		//int* int_ptr2 = engine::memory::New<int>(pool_allocator, 16);
		//REQUIRE(int_ptr2 != nullptr);
		//REQUIRE(int_ptr2[0] == 16);

		//int* int_ptr3 = engine::memory::New<int>(pool_allocator, 32);
		//REQUIRE(int_ptr3 != nullptr);
		//REQUIRE(int_ptr3[0] == 32);

		//int* int_ptr4 = engine::memory::New<int>(pool_allocator, 64);
		//REQUIRE(int_ptr4 != nullptr);
		//REQUIRE(int_ptr4[0] == 64);
	}

	SECTION("Containers with pool allocator")
	{
		//PoolAllocator<OneByteClass> pool_allocator(EMemoryCategory::Test, 1, 1, 512);
		//Vector<OneByteClass, PoolAllocator<OneByteClass>> one_byte_vector(512, pool_allocator);

		//for (size_t i = 0; i < 512; ++i)
		//{
		//	one_byte_vector.PushBack(OneByteClass());
		//}
	}

#if defined(BENCHMARKING)
	SECTION("Pool allocator benchmark (vs std's new)")
	{
		BENCHMARK("Pool allocator")
		{
			PoolAllocator<int, 1> pool_allocator(EMemoryCategory::Test, 2048);

			for (size_t i = 0; i < 512; ++i)
			{
				int* int_ptr = engine::memory::New<int>(pool_allocator, INT_MAX);
			}
		};
	}
#endif
}

TEST_CASE("Free list allocator", "[Memory]")
{
	using namespace engine::memory;
	using namespace engine::memory::tests;

	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types with free list allocator")
	{
		FreeListAllocator free_list_allocator(EMemoryCategory::Test, 16);

		int* int_ptr = engine::memory::New<int>(free_list_allocator, 16);
		REQUIRE(int_ptr != nullptr);
		REQUIRE(int_ptr[0] == 16);
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

		OneByteClass* one_byte = engine::memory::New<OneByteClass>(free_list_allocator, std::byte(64));
	}

	SECTION("Free list allocator with alignment")
	{
		FreeListAllocator free_list_allocator(EMemoryCategory::Test, 512);
		Vector<TwoByteClass, FreeListAllocator> two_byte_vector(free_list_allocator);

		for (size_t i = 0; i < 128; ++i)
		{
			TwoByteClass two_byte_class;
			two_byte_vector.PushBack(two_byte_class);
		}
	}

#if defined(BENCHMARKING)
	SECTION("Free list allocator benchmark (vs std's new)")
	{
		BENCHMARK("Free list allocator")
		{
			FreeListAllocator free_list_allocator(EMemoryCategory::Test, 2048);

			for (size_t i = 0; i < 256; ++i)
			{
				int* int_ptr = engine::memory::New<int>(free_list_allocator, INT_MAX);
			}

			for (size_t i = 0; i < 128; ++i)
			{
				double* float_ptr = engine::memory::New<double>(free_list_allocator, DBL_MAX);
			}
		};
	}
#endif
}

TEST_CASE("Slab allocator", "[Memory]")
{
	using namespace engine::memory;
	using namespace engine::memory::tests;

	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types")
	{
		SlabAllocator<2> slab_allocator(EMemoryCategory::Test);

	}

	SECTION("Aligned types")
	{

	}

	SECTION("Cotaneinrs with slab allocator")
	{

	}

#if defined(BENCHMARKING)
	SECTION("Slab allocator benchmark (vs std's new)")
	{
		SlabAllocator<2> slab_allocator(EMemoryCategory::Test);

		BENCHMARK("Slab allocator")
		{
			slab_allocator.CreateSlab<int>(2048);
			slab_allocator.CreateSlab<float>(2048);

			for (size_t i = 0; i < 512; ++i)
			{
				int* int_ptr = engine::memory::New<int>(slab_allocator, INT_MAX);
			}

			for (size_t i = 0; i < 512; ++i)
			{
				float* float_ptr = engine::memory::New<float>(slab_allocator, FLT_MAX);
			}

			slab_allocator.Release();
		};
	}
#endif
}