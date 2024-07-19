#include "engine/engine_pch.hpp"
#include "Context.hpp"

#include <iostream>

namespace engine
{
namespace rendering
{
	Context::Context()
	{
		CreateInstance();
	}

	void Context::FillInstanceLayers(std::vector<std::string>& _instance_layers, bool _verbose)
		{
			uint32_t layer_count{ 0 };
			VK_CHECK(vkEnumerateInstanceLayerProperties(&layer_count, nullptr));
			vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
			std::vector<VkLayerProperties> available_layers(layer_count);

			if (_verbose)
				std::cout << "Available instance layers:" << layer_count << "\n";
		}

		void Context::CreateInstance()
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

			vkCreateInstance(&create_info, nullptr, &m_VkInstance);
		}
	}
}