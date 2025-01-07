#pragma once

#include "engine/rendering/RendererType.hpp"

namespace engine::rendering
{
	class IRenderer
	{
		public:

			IRenderer();
			virtual ~IRenderer() = default;

			virtual void Initialize() = 0;
			virtual void BeginFrame() = 0;
			virtual void EndFrame() = 0;
			virtual void Shutdown() = 0;

			virtual bool ShouldClose() const = 0;

		protected:
				
			ERendererType m_RendererType;
	};
};