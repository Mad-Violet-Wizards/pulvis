#include "engine/engine_pch.hpp"
#include "RenderingService.hpp"

#include "vulkan/RendererVulkan.hpp"
#include "opengl/RendererOpenGL.hpp"

#include "engine/tools/imgui/ImGuiOpenGLRenderer.hpp"

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
			opengl::RendererOpenGL* rendererOpenGL = new opengl::RendererOpenGL();
			m_Renderer = rendererOpenGL;
			m_Renderer->Initialize();

			m_ImGuiRenderer = new tools::imgui::ImGuiOpenGLRenderer();
			m_ImGuiRenderer->Initialize(rendererOpenGL->GetWindow()->GetNativeWindow());
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

		if (GetImGuiRenderer())
		{
			GetImGuiRenderer()->BeginFrame();
		}
	}

	void RenderingService::Frame()
	{
		if (GetRenderer())
		{
			GetRenderer()->Frame();
		}

		if (GetImGuiRenderer())
		{
			ImGui::Begin("Debug");
			ImGui::Text("Hello, World!");
			ImGui::End();
		}
	}

	void RenderingService::EndFrame()
	{
		if (GetImGuiRenderer())
		{
			GetImGuiRenderer()->EndFrame();
		}

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

	tools::imgui::IImGuiRenderer* RenderingService::GetImGuiRenderer() const
	{
		return m_ImGuiRenderer;
	}
}

