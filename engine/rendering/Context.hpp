#pragma once

#include <vulkan/vulkan.h>

namespace engine
{
namespace rendering
{
	class Context
	{
		public:

		explicit Context();

		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

		void FillInstanceLayers(std::vector<std::string>& _instance_layers, bool _verbose);

	private:

		void CreateInstance();

	private:

		VkInstance m_VkInstance;
	};
}
}