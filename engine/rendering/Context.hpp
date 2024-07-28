#pragma once

#include "engine/Core/Export.hpp"

#include <vulkan/vulkan.h>

namespace engine
{
namespace rendering
{
	class PULVIS_API Context
	{
		public:

		explicit Context(const std::vector <std::string>& _requested_layers);

		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

	private:

		void FillInstanceLayers(std::vector<std::string>& _instance_layers, bool _verbose);
		void CreateInstance();

	private:

		VkInstance m_VkInstance;

		std::vector<std::string> m_VkEnabledInstanceLayers;
	};
}
}