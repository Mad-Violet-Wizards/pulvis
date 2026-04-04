#include "RenderSevice.hpp"
#include "opengl/GLRenderer.hpp"

namespace pulvis::rendering
{
	CRenderService::CRenderService(pulvis::fs::assets::CAssetRegistry& _asset_registry)
		: m_AssetRegistry(_asset_registry)
	{
	}

	CRenderService::~CRenderService()
	{
		if (m_Renderer)
		{
			m_Renderer->Shutdown();
		}
	}

	void CRenderService::Initialize(ERendererType _renderer_type,
		uint32_t _window_width, uint32_t _window_height,
		const std::string& _window_title)
	{
		switch (_renderer_type)
		{
			case ERendererType::OpenGL:
			{
				m_Renderer = std::make_unique<gl::CGLRenderer>(m_AssetRegistry);
				break;
			}
			case ERendererType::DirectX:
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
		if (m_Renderer)
		{
			m_Renderer->Shutdown();
			m_Renderer.reset();
		}
	}

	void CRenderService::Frame()
	{
		if (m_Renderer)
		{
			m_Renderer->BeginFrame();
			m_Renderer->Frame();
			m_Renderer->EndFrame();
		}
	}

	bool CRenderService::IsInitialized() const
	{
		return m_Renderer != nullptr;
	}

	bool CRenderService::GetShouldClose() const
	{
		return m_Renderer ? m_Renderer->GetShouldClose() : true;
	}

	IRenderer* CRenderService::GetRenderer()
	{
		return m_Renderer.get();
	}

	const IRenderer* CRenderService::GetRenderer() const
	{
		return m_Renderer.get();
	}
}