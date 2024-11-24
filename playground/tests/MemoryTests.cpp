#include "engine/engine_pch.hpp"
#include "MemoryTests.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#include "engine/memory/Memory.hpp"
#include "engine/memory/MemoryProfiler.hpp"
#include "engine/memory/Allocators/FrameAllocator.hpp"

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

	SECTION("Allocate & run out of memory")
	{
		FrameAllocator<OneByteClass> frame_allocator(EMemoryCategory::Test, 1024);

		for (size_t i = 0; i < 1024; ++i)
		{
			OneByteClass* one_byte = frame_allocator.allocate(1);
			REQUIRE(one_byte != nullptr);
		}
	}

	SECTION("Std::array with frame allocator.")
	{
		FrameAllocator<OneByteClass> frame_allocator(EMemoryCategory::Test, 1024);
		std::array<OneByteClass*, 1024> one_byte_array;

		for (size_t i = 0; i < 1024; ++i)
		{
			one_byte_array[i] = frame_allocator.allocate(1);
			REQUIRE(one_byte_array[i] != nullptr);
		}
	}

	SECTION("Release")
	{
		FrameAllocator<OneByteClass> frame_allocator(EMemoryCategory::Test, 8);
		OneByteClass* one_byte = frame_allocator.allocate(1);
		REQUIRE(one_byte != nullptr);
		frame_allocator.Release();
		REQUIRE(frame_allocator.allocate(1) == nullptr);
	}

	SECTION("Reset")
	{
		FrameAllocator<int> frame_allocator(EMemoryCategory::Test, 8);

		int* int_ptr_first = frame_allocator.allocate(1);
		REQUIRE(int_ptr_first != nullptr);

		int* int_ptr_second = frame_allocator.allocate(1);
		REQUIRE(int_ptr_second != nullptr);

		frame_allocator.Reset();

		int* int_ptr_third = frame_allocator.allocate(1);
		REQUIRE(int_ptr_third != nullptr);

	}

	SECTION("Zero Memory")
	{
		FrameAllocator<int> frame_allocator(EMemoryCategory::Test, 8);

		int* int_ptr = frame_allocator.allocate(1);
		*int_ptr = 128;
		REQUIRE(*int_ptr == 128);
		REQUIRE(int_ptr != nullptr);

		frame_allocator.ZeroMemory();

		REQUIRE(*int_ptr == 0);
	}
}