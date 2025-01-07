#pragma once

#include "engine/core/Export.hpp"
#include "engine/rendering/Renderer.hpp"

#include <vulkan/vulkan.h>

namespace engine
{
namespace rendering
{
	class CWindow;

	class CContext
	{
		public:
		
		CContext();

		explicit CContext(CWindow* _window, 
			const std::vector<std::string>& _requested_layers, 
			const std::vector<std::string>& _requested_instance_layers_extensions);

		CContext(const CContext&) = delete;
		CContext& operator=(const CContext&) = delete;

	private:

		void FillAvailableInstanceLayers(std::vector<std::string>& _instance_layers) const;
		void FillAvailableInstanceExtensions(std::vector<std::string>& _instance_extensions) const;
		void CreateInstance();

	private:

		VkInstance m_VkInstance;

		std::vector<std::string> m_VkEnabledInstanceLayers;
	};
}
}