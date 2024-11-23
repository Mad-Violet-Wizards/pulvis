#include "engine/engine_pch.hpp"
#include "Context.hpp"
#include "Utility.hpp"

#include <iostream>

namespace engine
{
namespace rendering
{
	CContext::CContext(const std::vector<std::string>& _requested_layers)
	{
		std::vector<std::string> vk_instance_layers;
		FillInstanceLayers(vk_instance_layers, engine::rendering::s_DebugVerbose);
		m_VkEnabledInstanceLayers = engine::pstd::Intersect(vk_instance_layers, _requested_layers);

		if (engine::rendering::s_DebugVerbose)
		{
			std::cout << "Enabled instance layers: \n";
			for (const auto& layer : m_VkEnabledInstanceLayers)
				std::cout << layer << "\n";
		}

		CreateInstance();
	}

	void CContext::FillInstanceLayers(std::vector<std::string>& _instance_layers, bool _verbose)
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
				if (_verbose)
					std::cout << "Available instance layer: " << layer.layerName << "\n";
			}
		}

		void CContext::CreateInstance()
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