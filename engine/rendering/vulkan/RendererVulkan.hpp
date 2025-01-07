#pragma once

#include "engine/rendering/Renderer.hpp"

namespace engine::rendering
{
	class CContext;
	class CWindow;

	class RendererVulkan : public IRenderer
	{
		public:

			RendererVulkan();
			~RendererVulkan();

			void Initialize() override;
			void BeginFrame() override;
			void EndFrame() override;
			void Shutdown() override;

			bool ShouldClose() const override;

	private:

			CContext* m_Context;
			CWindow* m_Window;
	};
}