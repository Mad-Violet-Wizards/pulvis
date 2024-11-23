#pragma once

#include <type_traits>
#include <thread>

namespace engine::threads
{
	using concurrency_t = std::invoke_result_t<decltype(std::thread::hardware_concurrency)>;
}