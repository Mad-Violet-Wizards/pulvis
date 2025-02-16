#pragma once

#include "engine/rendering/Renderer.hpp"
#include "engine/rendering/Window.hpp"

namespace engine::rendering::vulkan
{
	class CContext;

	class RendererVulkan : public IRenderer
	{
		public:

			RendererVulkan();
			~RendererVulkan();

			void Initialize() override;
			void BeginFrame() override;
			void Frame() override;
			void EndFrame() override;
			void Shutdown() override;

			bool ShouldClose() const override;

	private:

			CContext* m_Context;
			engine::rendering::CWindow* m_Window;
	};
}