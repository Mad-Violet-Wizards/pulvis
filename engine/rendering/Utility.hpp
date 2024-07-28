#pragma once

#include <vulkan/vulkan.h>


#define VK_CHECK(f) \
{ \
	const VkResult res = f; \
	if (res != VK_SUCCESS) \
	{ \
		const std::string message = "Vulkan API call failed with error code: " + std::to_string(res); \
		engine::core::CAssertManager::GetInstance().Assert(#f, message, __FILE__, __LINE__); \
	} \
} \

namespace engine
{
namespace rendering
{
	static bool s_DebugVerbose = true;
}
}