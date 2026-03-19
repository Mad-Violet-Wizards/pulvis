#pragma once

#include <cstdint>
#include <cstddef>
#include <thread>
#include <type_traits>

namespace pulvis::threads
{
	using thread_id_t = std::invoke_result_t<decltype(std::thread::hardware_concurrency)>;

	static constexpr size_t CACHE_LINE_SIZE = 64;
	static constexpr size_t DEFAULT_JOB_POOL_SIZE = 4096;
	static constexpr size_t DEFAULT_DEQUEUE_CAPACITY = 1024;
	static constexpr size_t DEFAULT_RING_BUFFER_CAPACITY = 65536;
}