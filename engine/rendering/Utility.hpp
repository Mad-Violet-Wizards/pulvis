#pragma once

#define VK_CHECK(f) \
{ \
	const VkResult res = f; \
	if (res != VK_SUCCESS) \
	{ \
		const std::string message = "Vulkan API call failed with error code: " + std::to_string(res); \
		engine::core::AssertManager::GetInstance().Assert(#f, message, __FILE__, __LINE__); \
	} \
} \