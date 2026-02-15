#pragma once

#include "RendererType.hpp"

namespace pulvis::rendering
{
	class IRenderer
	{
		public:

			IRenderer(ERendererType _renderer_type);
			virtual ~IRenderer() = default;

			virtual void Initialize() = 0;
			virtual void BeginFrame() = 0;
			virtual void Frame() = 0;
			virtual void EndFrame() = 0;
			virtual void Shutdown() = 0;
			virtual bool GetShouldClose() = 0;

			ERendererType GetType() const;

		protected:

			ERendererType m_RendererType;
	};
}