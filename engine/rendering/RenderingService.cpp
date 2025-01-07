#include "engine/engine_pch.hpp"
#include "RenderingService.hpp"

#include "vulkan/RendererVulkan.hpp"

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
					case ERendererType::Vulkan:
					{
						m_Renderer = new RendererVulkan();
						m_Renderer->Initialize();
						break;
					}
					default:
					{
						ASSERT(false, "Renderer type not supported!");
					}
				}
			}

			bool ShouldClose() const
			{
				return m_Renderer->ShouldClose();
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

	bool RenderingService::ShouldClose() const
	{
		return m_Impl->ShouldClose();
	}
}

