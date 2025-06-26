#include "engine/engine_pch.hpp"
#include "TestsMemoryMock.hpp"

#include <vendor/common/include/catch2/catch2.hpp>

#include "engine/experimental/memory/Memory.hpp"
#include "engine/experimental/memory/MemoryProfiler.hpp"
#include "engine/experimental/memory/Allocators/SlabAllocator.hpp"

#include "engine/experimental/containers/Vector.hpp"

using namespace engine::experimental::memory;
using namespace tests::experimental::memory;
using namespace engine::experimental::containers;

TEST_CASE("Slab allocator", "[Memory]")
{
	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types")
	{
		SlabAllocator<2> slab_allocator(EMemoryCategory::Test);
		slab_allocator.CreateSlab<int>(2048);
		slab_allocator.CreateSlab<float>(2048);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(slab_allocator, 16);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == 16);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			float* float_ptr = engine::experimental::memory::New<float>(slab_allocator, 32.5f);
			REQUIRE(float_ptr != nullptr);
			REQUIRE(float_ptr[0] == 32.5f);
		}
	}

	SECTION("Allocations and deallocations")
	{
		SlabAllocator<2> slab_allocator(EMemoryCategory::Test);
		slab_allocator.CreateSlab<int>(2048);
		slab_allocator.CreateSlab<float>(2048);

		constexpr int first_loop_int_cstr_arg = 882'352;
		constexpr int second_loop_int_cstr_arg = 436'282;
		constexpr int third_loop_int_cstr_arg = 198'731;

		constexpr float first_loop_float_cstr_arg = 32.521f;
		constexpr float second_loop_float_cstr_arg = 96.226f;
		constexpr float third_loop_float_cstr_arg = 1.052f;

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(slab_allocator, first_loop_int_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == first_loop_int_cstr_arg);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			float* float_ptr = engine::experimental::memory::New<float>(slab_allocator, first_loop_float_cstr_arg);
			REQUIRE(float_ptr != nullptr);
			REQUIRE(float_ptr[0] == first_loop_float_cstr_arg);
		}

		slab_allocator.ResetSlab<int>();
		slab_allocator.ResetSlab<float>();

		std::vector<int*> int_ptrs;
		std::vector<float*> float_ptrs;

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(slab_allocator, second_loop_int_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == second_loop_int_cstr_arg);
			int_ptrs.push_back(int_ptr);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			float* float_ptr = engine::experimental::memory::New<float>(slab_allocator, second_loop_float_cstr_arg);
			REQUIRE(float_ptr != nullptr);
			REQUIRE(float_ptr[0] == second_loop_float_cstr_arg);
			float_ptrs.push_back(float_ptr);
		}

		for (size_t i = 0; i < int_ptrs.size(); ++i)
		{
			engine::experimental::memory::Delete(slab_allocator, int_ptrs[i]);
		}

		for (size_t i = 0; i < float_ptrs.size(); ++i)
		{
			engine::experimental::memory::Delete(slab_allocator, float_ptrs[i]);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::experimental::memory::New<int>(slab_allocator, third_loop_int_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == third_loop_int_cstr_arg);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			float* float_ptr = engine::experimental::memory::New<float>(slab_allocator, third_loop_float_cstr_arg);
			REQUIRE(float_ptr != nullptr);
			REQUIRE(float_ptr[0] == third_loop_float_cstr_arg);
		}
	}

	SECTION("Irregular types (no alignment)")
	{
		SlabAllocator<2> slab_allocator(EMemoryCategory::Test);
		slab_allocator.CreateSlab<ThreeByteClass>(512);
		slab_allocator.CreateSlab<FourtyTwoByteClass>(512);

		for (size_t i = 0; i < 128; ++i)
		{
			ThreeByteClass* three_byte_class = engine::experimental::memory::New<ThreeByteClass>(slab_allocator);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			FourtyTwoByteClass* fourty_two_byte_class = engine::experimental::memory::New<FourtyTwoByteClass>(slab_allocator);
		}
	}

	SECTION("Cotaneinrs with slab allocator")
	{
		SlabAllocator<2> slab_allocator(EMemoryCategory::Test);
		slab_allocator.CreateSlab<OneByteClass>(512);

		Vector<OneByteClass, SlabAllocator<2>> one_byte_vector(512, slab_allocator);

		for (size_t i = 0; i < 512; ++i)
		{
			one_byte_vector.PushBack(OneByteClass());
		}
	}
}