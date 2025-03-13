#pragma once

#include "RendererType.hpp"
#include "Renderer.hpp"

namespace engine::rendering
{
	class RenderingService : public Singleton<RenderingService>
	{
		public:

			~RenderingService();

			void Initialize(ERendererType _rendererType);

			void BeginFrame();
			void Frame();
			void EndFrame();

			bool ShouldClose() const;
			bool IsInitialized() const;

			IRenderer* GetRenderer() const;

		private:

			friend class Singleton<RenderingService>;

			RenderingService();

			IRenderer* m_Renderer{ nullptr };
	};
}
