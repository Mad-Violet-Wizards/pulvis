#pragma once

#include "engine/core/Export.hpp"
#include "engine/rendering/Renderer.hpp"

#include <vulkan/vulkan.h>

namespace engine::rendering
{
	class CWindow;

	namespace vulkan
	{
		class CContext
		{
		public:

			CContext();

			explicit CContext(engine::rendering::CWindow* _window,
				const std::vector<std::string>& _requested_layers,
				const std::vector<std::string>& _requested_instance_layers_extensions,
				const std::vector<std::string>& _requested_device_extensions);

			CContext(const CContext&) = delete;
			CContext& operator=(const CContext&) = delete;

		private:

			void FillAvailableInstanceLayers(std::vector<std::string>& _instance_layers) const;
			void FillAvailableInstanceExtensions(std::vector<std::string>& _instance_extensions) const;

			void CreateInstance(uint32_t _enabled_layers_count, const char* const* _enabled_layers_names, uint32_t _enabled_layer_extensions_count, const char* const* _enabled_layer_extensions_names);

			void FillAvailablePhysicalDevices(const std::vector<std::string>& _requested_device_extensions, std::vector<VkPhysicalDevice>& _physical_devices) const;
			bool IsPhysicalDeviceSuitable(VkPhysicalDevice _device) const;

			struct SQueueFamilyIndices
			{
				uint32_t m_GraphicsFamily{ UINT32_MAX };
				uint32_t m_PresentFamily{ UINT32_MAX };

				bool IsComplete() const
				{
					return m_GraphicsFamily != UINT32_MAX && m_PresentFamily != UINT32_MAX;
				}
			};

			SQueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _device) const;

		private:

			VkInstance m_VkInstance;
			VkSurfaceKHR m_VkSurface;
			VkPhysicalDevice m_VkPhysicalDevice;
		};
	}
}