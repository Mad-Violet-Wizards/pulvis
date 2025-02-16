#pragma once

#include <vulkan/vulkan.h>

namespace engine::rendering::vulkan
{
	class CPhysicalDevice
	{
	public:

		CPhysicalDevice();
		explicit CPhysicalDevice(VkPhysicalDevice _physical_device, VkSurfaceKHR _vk_surface, const std::vector<std::string>& _requested_device_extensions);
		~CPhysicalDevice();

		CPhysicalDevice(const CPhysicalDevice&) = delete;
		CPhysicalDevice& operator=(const CPhysicalDevice&) = delete;

		VkPhysicalDevice GetPhysicalDevice() const { return m_VkPhysicalDevice; }


	private:

		VkPhysicalDevice m_VkPhysicalDevice;
	};
}