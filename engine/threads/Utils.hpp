#pragma once

// Inspired by
// * https://github.com/bshoshany/thread-pool/
// * https://github.com/inkooboo/thread-pool-cpp/
// * https://gist.github.com/holtgrewe/8728757

#include <type_traits>
#include <thread>

namespace engine::threads
{
	using concurrency_t = std::invoke_result_t<decltype(std::thread::hardware_concurrency)>;
}