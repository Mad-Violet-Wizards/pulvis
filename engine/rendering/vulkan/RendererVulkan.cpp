#include "engine/engine_pch.hpp"
#include "RendererVulkan.hpp"

#include <windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include <GLFW/glfw3.h>

#include "ContextVulkan.hpp"
#include "engine/core/Application.hpp"


namespace engine::rendering::vulkan
{
	RendererVulkan::RendererVulkan()
		: engine::rendering::IRenderer()
		, m_Context{ nullptr }
		, m_Window{ nullptr }
	{
		m_RendererType = ERendererType::Vulkan;
	}

	RendererVulkan::~RendererVulkan()
	{
		delete m_Context;
		delete m_Window;
	}

	void RendererVulkan::Initialize()
	{
		static const engine::core::CApplicationContext& app_context = engine::core::Application::GetContext();

		const std::string window_name = app_context.m_Setup.m_ApplicationName;

		m_Window = new engine::rendering::CWindow(app_context.m_Setup.m_WindowWidth,
			app_context.m_Setup.m_WindowHeight, 
			window_name.c_str());

		const std::vector<std::string> requested_instance_layers
		{
			"VK_LAYER_KHRONOS_validation"
		};

 		const std::vector<std::string> requested_instance_layers_extensions
		{
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		};
		
		const std::vector<std::string> device_extensions
		{
			VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME
		};

		m_Context = new CContext(m_Window, requested_instance_layers, requested_instance_layers_extensions, device_extensions);
	}

	void RendererVulkan::BeginFrame()
	{
	}

	void RendererVulkan::Frame()
	{
	}

	void RendererVulkan::EndFrame()
	{
	}

	void RendererVulkan::Shutdown()
	{
	}

	bool RendererVulkan::ShouldClose() const
	{
		return m_Window->ShouldClose();
	}
}

