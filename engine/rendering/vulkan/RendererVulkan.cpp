#include "engine/engine_pch.hpp"
#include "RendererVulkan.hpp"

#include <windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "Context.hpp"
#include "Window.hpp"
#include "engine/core/Application.hpp"


namespace engine::rendering
{
	RendererVulkan::RendererVulkan()
		: IRenderer()
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
		const std::string window_name = engine::core::s_AppContext.m_AppSetup.m_ApplicationName;

		m_Window = new rendering::CWindow(engine::core::s_AppContext.m_AppSetup.m_WindowWidth, 
			engine::core::s_AppContext.m_AppSetup.m_WindowHeight, 
			window_name.c_str());

		const std::vector<std::string> requested_instance_layers
		{
		};

 		const std::vector<std::string> requested_instance_layers_extensions
		{
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		};

		m_Context = new rendering::CContext(m_Window, requested_instance_layers, requested_instance_layers_extensions);
	}

	void RendererVulkan::BeginFrame()
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

