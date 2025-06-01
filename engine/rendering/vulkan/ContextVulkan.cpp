#include "engine/engine_pch.hpp"
#include "ContextVulkan.hpp"

#include "engine/rendering/Window.hpp"
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Utility.hpp"

#include <iostream>

namespace engine::rendering::vulkan
{
	CContext::CContext()
		: m_VkInstance(VK_NULL_HANDLE)
		, m_VkSurface(VK_NULL_HANDLE)
		, m_VkPhysicalDevice(VK_NULL_HANDLE)
	{
	}

	CContext::CContext(engine::rendering::CWindow* _window,
		const std::vector<std::string>& _requested_layers,
		const std::vector<std::string>& _requested_instance_layers_extensions,
		const std::vector<std::string>& _requested_device_extensions)
	{
		std::vector<std::string> instance_layers;
		std::vector<std::string> enabled_instance_layers;
		FillAvailableInstanceLayers(instance_layers);
		enabled_instance_layers = engine::pstd::Intersect(_requested_layers, instance_layers);
		std::vector<const char*> enabled_instance_layers_cstr;
		enabled_instance_layers_cstr.reserve(enabled_instance_layers.size());

		std::vector<std::string> instance_extensions;
		std::vector<std::string> enabled_instance_extensions;
		FillAvailableInstanceExtensions(instance_extensions);
		enabled_instance_extensions = engine::pstd::Intersect(_requested_instance_layers_extensions, instance_extensions);
		std::vector<const char*> enabled_instance_extensions_cstr;

		for (const auto& layer : enabled_instance_layers)
			enabled_instance_layers_cstr.push_back(layer.c_str());

		enabled_instance_extensions_cstr.reserve(enabled_instance_extensions.size());
		for (const auto& extension : enabled_instance_extensions)
			enabled_instance_extensions_cstr.push_back(extension.c_str());

		if (engine::rendering::s_DebugVerbose)
		{
			std::cout << "Enabled instance layers: \n";
			for (const auto& layer : enabled_instance_layers)
				std::cout << layer << "\n";
		}

		if (engine::rendering::s_DebugVerbose)
		{
			std::cout << "Enabled instance extensions: \n";
			for (const auto& extension : enabled_instance_extensions)
				std::cout << extension << "\n";
		}

		CreateInstance(static_cast<uint32_t>(enabled_instance_layers_cstr.size()), 
									 enabled_instance_layers_cstr.data(),
									 static_cast<uint32_t>(enabled_instance_extensions_cstr.size()), 
									 enabled_instance_extensions_cstr.data());

#if defined (WINDOWS_OS)
		const bool windows_surface_extension = std::find(enabled_instance_extensions.begin(), enabled_instance_extensions.end(), VK_KHR_WIN32_SURFACE_EXTENSION_NAME) != enabled_instance_extensions.end();

		ASSERT(windows_surface_extension, "Windows surface extension not enabled!");
		if (windows_surface_extension)
		{
			GLFWwindow* glfw_window = _window->GetNativeWindow();
			HWND window_os_handle = glfwGetWin32Window(glfw_window);

			VkWin32SurfaceCreateInfoKHR surface_create_info
			{
				.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
				.hinstance = GetModuleHandle(nullptr),
				.hwnd = window_os_handle
			};

			VK_CHECK(vkCreateWin32SurfaceKHR(m_VkInstance, &surface_create_info, nullptr, &m_VkSurface));
		}
#endif

		if (engine::rendering::s_DebugVerbose)
			std::cout << "Vulkan context created!\n";
		
		std::vector<VkPhysicalDevice> physical_devices;
		FillAvailablePhysicalDevices(_requested_device_extensions, physical_devices);

		for (const auto& device : physical_devices)
		{
			if (IsPhysicalDeviceSuitable(device))
			{
				m_VkPhysicalDevice = device;
				break;
			}
		}

		ASSERT(m_VkPhysicalDevice != VK_NULL_HANDLE, "No suitable physical device found!");
		if (engine::rendering::s_DebugVerbose)
		{
			std::cout << "Physical device found!\n";

			VkPhysicalDeviceProperties device_properties;
			vkGetPhysicalDeviceProperties(m_VkPhysicalDevice, &device_properties);

			std::cout << "Device Name: " << device_properties.deviceName << "\n";
		}
	}

	void CContext::FillAvailableInstanceLayers(std::vector<std::string>& _instance_layers) const
	{
		uint32_t layer_count{ 0 };
		VK_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		std::vector<VkLayerProperties> available_layers;
		available_layers.resize(layer_count);
		VK_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data()));

		std::cout << "Layers count: " << layer_count << "\n";
		for (const auto& layer : available_layers)
		{
			_instance_layers.push_back(layer.layerName);
			if (engine::rendering::s_DebugVerbose)
				std::cout << "Available instance layer: " << layer.layerName << "\n";
		}
	}

	void CContext::FillAvailableInstanceExtensions(std::vector<std::string>& _instance_extensions) const
	{
		uint32_t extension_count{ 0 };
		VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr));
		std::vector<VkExtensionProperties> available_extensions;
		available_extensions.resize(extension_count);
		VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data()));

		std::cout << "Extensions count: " << extension_count << "\n";
		for (const auto& extension : available_extensions)
		{
			_instance_extensions.push_back(extension.extensionName);
			if (engine::rendering::s_DebugVerbose)
				std::cout << "Available instance extension: " << extension.extensionName << "\n";
		}
	}

	void CContext::FillAvailablePhysicalDevices(const std::vector<std::string>& _requested_device_extensions, std::vector<VkPhysicalDevice>& _physical_devices) const
	{
		uint32_t physical_device_count{ 0 };
		VK_CHECK(vkEnumeratePhysicalDevices(m_VkInstance, &physical_device_count, nullptr));
		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		VK_CHECK(vkEnumeratePhysicalDevices(m_VkInstance, &physical_device_count, physical_devices.data()));

		for (const auto& device : physical_devices)
		{
			_physical_devices.emplace_back(device);
		}
	}

	bool CContext::IsPhysicalDeviceSuitable(VkPhysicalDevice _device) const
	{
		VkPhysicalDeviceProperties device_properties;
		vkGetPhysicalDeviceProperties(_device, &device_properties);

		VkPhysicalDeviceFeatures device_features;
		vkGetPhysicalDeviceFeatures(_device, &device_features);
		
		SQueueFamilyIndices indices = FindQueueFamilies(_device);
		return indices.IsComplete();
	}

	CContext::SQueueFamilyIndices CContext::FindQueueFamilies(VkPhysicalDevice _device) const
	{
		SQueueFamilyIndices indices;

		uint32_t queue_family_count = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &queue_family_count, queue_families.data());

		int i = 0;
		for (const auto& queue_family : queue_families)
		{
			if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.m_GraphicsFamily = i;

			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_device, i, m_VkSurface, &present_support);

			if (present_support)
				indices.m_PresentFamily = i;

			if (indices.IsComplete())
				break;

			++i;
		}

		return indices;
	}

	void CContext::CreateInstance(uint32_t _enabled_layers_count, 
																const char* const* _enabled_layers_names,
																uint32_t _enabled_layer_extensions_count, const char* 
																const* _enabled_layer_extensions_names)
	{
		VkApplicationInfo app_info{};
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName = "Pulvis Engine Alpha (1.0.0)";
		app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.pEngineName = "Pulvis Engine";
		app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		app_info.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;
		create_info.enabledLayerCount = _enabled_layers_count;
		create_info.ppEnabledLayerNames = _enabled_layers_names;
		create_info.enabledExtensionCount = _enabled_layer_extensions_count;
		create_info.ppEnabledExtensionNames = _enabled_layer_extensions_names;

		vkCreateInstance(&create_info, nullptr, &m_VkInstance);
	}
}