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
			bool ShouldClose() const;

		private:

			friend class Singleton<RenderingService>;

			RenderingService();

			class Impl;
			Impl* m_Impl;
	};
}
