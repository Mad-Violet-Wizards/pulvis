#pragma once

#include "engine/core/Export.hpp"

#include <vulkan/vulkan.h>

namespace engine
{
namespace rendering
{
	class CContext
	{
		public:

		explicit CContext(const std::vector <std::string>& _requested_layers);

		CContext(const CContext&) = delete;
		CContext& operator=(const CContext&) = delete;

	private:

		void FillInstanceLayers(std::vector<std::string>& _instance_layers, bool _verbose);
		void CreateInstance();

	private:

		VkInstance m_VkInstance;

		std::vector<std::string> m_VkEnabledInstanceLayers;
	};
}
}