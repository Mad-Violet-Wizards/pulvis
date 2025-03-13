#include "engine/engine_pch.hpp"
#include "RenderingService.hpp"

#include "vulkan/RendererVulkan.hpp"
#include "opengl/RendererOpenGL.hpp"

namespace engine::rendering
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	RenderingService::RenderingService()
	{
	}

	RenderingService::~RenderingService()
	{
	}

	void RenderingService::Initialize(ERendererType _rendererType)
	{
		switch (_rendererType)
		{
		case ERendererType::OpenGL:
		{
			m_Renderer = new opengl::RendererOpenGL();
			m_Renderer->Initialize();
			break;
		}
		case ERendererType::Vulkan:
		{
			m_Renderer = new vulkan::RendererVulkan();
			m_Renderer->Initialize();
			break;
		}
		default:
		{
			ASSERT(false, "Renderer type not supported!");
		}
		}
	}

	void RenderingService::BeginFrame()
	{
		if (GetRenderer())
		{
			GetRenderer()->BeginFrame();
		}
	}

	void RenderingService::Frame()
	{
		if (GetRenderer())
		{
			GetRenderer()->Frame();
		}
	}

	void RenderingService::EndFrame()
	{
		if (GetRenderer())
		{
			GetRenderer()->EndFrame();
		}
	}

	bool RenderingService::ShouldClose() const
	{
		return m_Renderer->ShouldClose();
	}

	bool RenderingService::IsInitialized() const
	{
		return m_Renderer != nullptr;
	}

	IRenderer* RenderingService::GetRenderer() const
	{
		return m_Renderer;
	}
}

