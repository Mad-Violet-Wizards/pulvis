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

#undef BENCHMARKING
#if defined(BENCHMARKING)
namespace engine::memory::tests
{
	constexpr size_t BENCHMARK_ITERATIONS = 512;
	constexpr size_t ALLOCATOR_BENCHMARK_SIZE = 2048;
}
#endif

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

	SECTION("Irregular types")
	{
		FrameAllocator<ThreeByteClass> frame_allocator_three_bytes(EMemoryCategory::Test, 512);
		FrameAllocator<FourtyTwoByteClass> frame_allocators_fourty_two_bytes(EMemoryCategory::Test, 512);

		for (size_t i = 0; i < 128; ++i)
		{
			ThreeByteClass* three_byte_class = engine::memory::New<ThreeByteClass>(frame_allocator_three_bytes);
			REQUIRE(three_byte_class != nullptr);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			FourtyTwoByteClass* fourty_two_byte_class = engine::memory::New<FourtyTwoByteClass>(frame_allocators_fourty_two_bytes);
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

#if defined(BENCHMARKING)
	SECTION("Frame allocator benchmark")
	{
		static size_t FRAME_ALLOCATOR_BENCHMARK_ALLOCATIONS = 0;

		BENCHMARK("Frame allocator")
		{
			FrameAllocator<int> frame_allocator(EMemoryCategory::Test, ALLOCATOR_BENCHMARK_SIZE);

			for (size_t i = 0; i < BENCHMARK_ITERATIONS; ++i)
			{
				int* int_ptr = engine::memory::New<int>(frame_allocator);
				FRAME_ALLOCATOR_BENCHMARK_ALLOCATIONS++;
			}
		};

		std::cout << "\nFrame allocator benchmark allocations: " << FRAME_ALLOCATOR_BENCHMARK_ALLOCATIONS << "\n";
		std::cout << "Frame allocator allocated a total of: " << FRAME_ALLOCATOR_BENCHMARK_ALLOCATIONS * sizeof(int) << " bytes\n";
	}
#endif
}

TEST_CASE("Pool allocator", "[Memory]")
{
	using namespace engine::memory;
	using namespace engine::memory::tests;

	engine::core::CAssertManager::GetInstance().SetActive(false);

	SECTION("Simple types allocation with pool allocator")
	{
		PoolAllocator<int, 2> pool_allocator(EMemoryCategory::Test, 1024);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(pool_allocator, 16);
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
			int* int_ptr = engine::memory::New<int>(pool_allocator, first_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == first_loop_cstr_arg);
		}

		pool_allocator.Reset();
		std::vector<int*> ptrs;
		size_t predicted_free_indexes = 0;
		std::vector<int> where;

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(pool_allocator, second_loop_cstr_arg);
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
			engine::memory::Delete(pool_allocator, ptrs[i]);
		}

		for (size_t i = 0; i < predicted_free_indexes; ++i)
		{
			int* int_ptr = engine::memory::New<int>(pool_allocator, third_loop_cstr_arg);
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
			ThreeByteClass* three_byte_class = engine::memory::New<ThreeByteClass>(pool_allocator_three_bytes);
			REQUIRE(three_byte_class != nullptr);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			FourtyTwoByteClass* fourty_two_byte_class = engine::memory::New<FourtyTwoByteClass>(pool_allocators_fourty_two_bytes);
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

#if defined(BENCHMARKING)
	SECTION("Pool allocator benchmark")
	{
		static size_t POOL_ALLOCATOR_BENCHMARK_ALLOCATIONS = 0;

		BENCHMARK("Pool allocator")
		{
			PoolAllocator<int, 1> pool_allocator(EMemoryCategory::Test, ALLOCATOR_BENCHMARK_SIZE);

			for (size_t i = 0; i < BENCHMARK_ITERATIONS; ++i)
			{
				int* int_ptr = engine::memory::New<int>(pool_allocator, INT_MAX);
				POOL_ALLOCATOR_BENCHMARK_ALLOCATIONS++;
			}
		};

		std::cout << "\nPool allocator benchmark allocations: " << POOL_ALLOCATOR_BENCHMARK_ALLOCATIONS << "\n";
		std::cout << "Pool allocator allocated a total of: " << POOL_ALLOCATOR_BENCHMARK_ALLOCATIONS * sizeof(int) << " bytes\n";
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
		FreeListAllocator free_list_allocator(EMemoryCategory::Test, 2048);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(free_list_allocator, 16);
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
			int* int_ptr = engine::memory::New<int>(free_list_allocator, first_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == first_loop_cstr_arg);
		}

		free_list_allocator.Reset();
		std::vector<int*> ptrs;

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(free_list_allocator, second_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == second_loop_cstr_arg);
			ptrs.push_back(int_ptr);
		}

		for (size_t i = 0; i < ptrs.size(); ++i)
		{
			engine::memory::Delete(free_list_allocator, ptrs[i]);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(free_list_allocator, third_loop_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == third_loop_cstr_arg);
		}
	}

	SECTION("Irregular types")
	{
		FreeListAllocator free_list_allocator(EMemoryCategory::Test, 1024);

		for (size_t i = 0; i < 128; ++i)
		{
			ThreeByteClass* three_byte_class = engine::memory::New<ThreeByteClass>(free_list_allocator);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			FourtyTwoByteClass* fourty_two_byte_class = engine::memory::New<FourtyTwoByteClass>(free_list_allocator);
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

#if defined(BENCHMARKING)
	SECTION("Free list allocator benchmark")
	{
		static size_t FREE_LIST_ALLOCATOR_BENCHMARK_ALLOCATIONS = 0;

		BENCHMARK("Free list allocator")
		{
			FreeListAllocator free_list_allocator(EMemoryCategory::Test, ALLOCATOR_BENCHMARK_SIZE);

			for (size_t i = 0; i < BENCHMARK_ITERATIONS; ++i)
			{
				int* int_ptr = engine::memory::New<int>(free_list_allocator, INT_MAX);
				FREE_LIST_ALLOCATOR_BENCHMARK_ALLOCATIONS++;
			}
		};

		std::cout << "\nFree list allocator benchmark allocations: " << FREE_LIST_ALLOCATOR_BENCHMARK_ALLOCATIONS << "\n";
		std::cout << "Free list allocator allocated a total of: " << FREE_LIST_ALLOCATOR_BENCHMARK_ALLOCATIONS * sizeof(int) << " bytes\n";
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
		slab_allocator.CreateSlab<int>(2048);
		slab_allocator.CreateSlab<float>(2048);

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(slab_allocator, 16);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == 16);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			float* float_ptr = engine::memory::New<float>(slab_allocator, 32.5f);
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
			int* int_ptr = engine::memory::New<int>(slab_allocator, first_loop_int_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == first_loop_int_cstr_arg);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			float* float_ptr = engine::memory::New<float>(slab_allocator, first_loop_float_cstr_arg);
			REQUIRE(float_ptr != nullptr);
			REQUIRE(float_ptr[0] == first_loop_float_cstr_arg);
		}

		slab_allocator.ResetSlab<int>();
		slab_allocator.ResetSlab<float>();

		std::vector<int*> int_ptrs;
		std::vector<float*> float_ptrs;

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(slab_allocator, second_loop_int_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == second_loop_int_cstr_arg);
			int_ptrs.push_back(int_ptr);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			float* float_ptr = engine::memory::New<float>(slab_allocator, second_loop_float_cstr_arg);
			REQUIRE(float_ptr != nullptr);
			REQUIRE(float_ptr[0] == second_loop_float_cstr_arg);
			float_ptrs.push_back(float_ptr);
		}

		for (size_t i = 0; i < int_ptrs.size(); ++i)
		{
			engine::memory::Delete(slab_allocator, int_ptrs[i]);
		}

		for (size_t i = 0; i < float_ptrs.size(); ++i)
		{
			engine::memory::Delete(slab_allocator, float_ptrs[i]);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			int* int_ptr = engine::memory::New<int>(slab_allocator, third_loop_int_cstr_arg);
			REQUIRE(int_ptr != nullptr);
			REQUIRE(int_ptr[0] == third_loop_int_cstr_arg);
		}

		for (size_t i = 0; i < 512; ++i)
		{
			float* float_ptr = engine::memory::New<float>(slab_allocator, third_loop_float_cstr_arg);
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
			ThreeByteClass* three_byte_class = engine::memory::New<ThreeByteClass>(slab_allocator);
		}

		for (size_t i = 0; i < 8; ++i)
		{
			FourtyTwoByteClass* fourty_two_byte_class = engine::memory::New<FourtyTwoByteClass>(slab_allocator);
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

#if defined(BENCHMARKING)
	SECTION("Slab allocator benchmark")
	{
		SlabAllocator<2> slab_allocator(EMemoryCategory::Test);
		static size_t SLAB_ALLOCATOR_BENCHMARK_ALLOCATIONS = 0;

		BENCHMARK("Slab allocator")
		{
			slab_allocator.CreateSlab<int>(ALLOCATOR_BENCHMARK_SIZE);

			for (size_t i = 0; i < BENCHMARK_ITERATIONS; ++i)
			{
				int* int_ptr = engine::memory::New<int>(slab_allocator, INT_MAX);
				SLAB_ALLOCATOR_BENCHMARK_ALLOCATIONS++;
			}

			slab_allocator.Release();
		};

		std::cout << "\nSlab allocator benchmark allocations: " << SLAB_ALLOCATOR_BENCHMARK_ALLOCATIONS << "\n";
		std::cout << "Slab allocator allocated a total of: " << SLAB_ALLOCATOR_BENCHMARK_ALLOCATIONS * sizeof(int) << " bytes\n";
	}
#endif
}

#if defined(BENCHMARKING)
TEST_CASE("std::new / delete")
{
	BENCHMARK("std::new / delete")
	{
		for (size_t i = 0; i < engine::memory::tests::BENCHMARK_ITERATIONS; ++i)
		{
			int* int_ptr = new int(INT_MAX);
			delete int_ptr;
		}
	};

	BENCHMARK("Shared pointers")
	{
		for (size_t i = 0; i < engine::memory::tests::BENCHMARK_ITERATIONS; ++i)
		{
			std::shared_ptr<int> int_ptr = std::make_unique<int>(INT_MAX);
		}
	};
}
#endif