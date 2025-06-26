#include "engine/engine_pch.hpp"
#include "TestsMemoryMock.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#include "engine/experimental/memory/Memory.hpp"
#include "engine/experimental/memory/MemoryProfiler.hpp"
#include "engine/experimental/memory/Allocators/FrameAllocator.hpp"

#include "engine/experimental/containers/Vector.hpp"

using namespace engine::experimental::memory;
using namespace tests::experimental::memory;
using namespace engine::experimental::containers;

TEST_CASE("Frame Allocator", "[Memory]")
{
	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types with frame alloactor")
	{
		FrameAllocator<int> frame_allocator(EMemoryCategory::Test, 2048);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(frame_allocator);
			REQUIRE(int_ptr != nullptr);
		}
	}

	SECTION("Irregular types")
	{
		FrameAllocator<ThreeByteClass> frame_allocator_three_bytes(EMemoryCategory::Test, 512);
		FrameAllocator<FourtyTwoByteClass> frame_allocators_fourty_two_bytes(EMemoryCategory::Test, 512);

		for (size_t i = 0; i < 128; ++i)
		{
			ThreeByteClass* three_byte_class = engine::experimental::memory::New<ThreeByteClass>(frame_allocator_three_bytes);
			REQUIRE(three_byte_class != nullptr);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			FourtyTwoByteClass* fourty_two_byte_class = engine::experimental::memory::New<FourtyTwoByteClass>(frame_allocators_fourty_two_bytes);
			REQUIRE(fourty_two_byte_class != nullptr);
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
}