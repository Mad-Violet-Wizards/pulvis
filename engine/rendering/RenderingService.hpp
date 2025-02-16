#pragma once

#include "RendererType.hpp"
#include "Renderer.hpp"

namespace engine::rendering
{
	class PULVIS_API RenderingService : public Singleton<RenderingService>
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

			class Impl;
			Impl* m_Impl;
	};
}
