#include "RenderSevice.hpp"

#include "opengl/GLRenderer.hpp"
#include <iostream>

namespace pulvis::rendering
{
	CRenderService& CRenderService::GetInstance()
	{
		static CRenderService instance;
		return instance;
	}

	void CRenderService::Initialize(ERendererType _renderer_type)
	{
		switch (_renderer_type)
		{
			case ERendererType::OpenGL:
			{
				m_Renderer = new gl::CGLRenderer();
				break;
			}
			case ERendererType::DirectX:
			{
				break;
			}
			case ERendererType::Vulkan:
			{
				break;
			}
		}

		if (m_Renderer)
		{
			m_Renderer->Initialize();
		}
	}

	void CRenderService::Shutdown()
	{
		m_Renderer->Shutdown();
		delete m_Renderer;
	}

	void CRenderService::Frame()
	{
		m_Renderer->BeginFrame();
		m_Renderer->Frame();
		m_Renderer->EndFrame();
	}

	bool CRenderService::IsInitialized() const
	{
		return m_Renderer != nullptr;
	}

	bool CRenderService::GetShouldClose() const
	{
		return m_Renderer->GetShouldClose();
	}

	IRenderer* CRenderService::GetRenderer()
	{
		return m_Renderer;
	}

	const IRenderer* CRenderService::GetRenderer() const
	{
		return m_Renderer;
	}
}

