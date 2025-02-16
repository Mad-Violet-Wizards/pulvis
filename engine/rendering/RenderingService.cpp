#include "engine/engine_pch.hpp"
#include "RenderingService.hpp"

#include "vulkan/RendererVulkan.hpp"
#include "opengl/RendererOpenGL.hpp"

namespace engine::rendering
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	class RenderingService::Impl
	{
		public:

			void Initialize(ERendererType _rendererType)
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

			IRenderer* GetRenderer() const
			{
				return m_Renderer;
			}

			bool IsRendererInitialized() const
			{
				return m_Renderer != nullptr;
			}

			bool ShouldClose() const
			{
				if (m_Renderer)
				{
					return m_Renderer->ShouldClose();
				}

				return false;
			}

		private:

			IRenderer* m_Renderer{ nullptr };
	};
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	RenderingService::RenderingService()
	{
		m_Impl = new Impl();
	}

	RenderingService::~RenderingService()
	{
		delete m_Impl;
	}

	void RenderingService::Initialize(ERendererType _rendererType)
	{
		m_Impl->Initialize(_rendererType);
	}

	void RenderingService::BeginFrame()
	{
		if (m_Impl->GetRenderer())
		{
			m_Impl->GetRenderer()->BeginFrame();
		}
	}

	void RenderingService::Frame()
	{
		if (m_Impl->GetRenderer())
		{
			m_Impl->GetRenderer()->Frame();
		}
	}

	void RenderingService::EndFrame()
	{
		if (m_Impl->GetRenderer())
		{
			m_Impl->GetRenderer()->EndFrame();
		}
	}

	bool RenderingService::ShouldClose() const
	{
		return m_Impl->ShouldClose();
	}

	bool RenderingService::IsInitialized() const
	{
		return m_Impl->IsRendererInitialized();
	}

	IRenderer* RenderingService::GetRenderer() const
	{
		return m_Impl->GetRenderer();
	}
}

